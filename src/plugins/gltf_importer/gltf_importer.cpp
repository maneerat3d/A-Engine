#include "gltf_importer.h"
#include "src/renderer/mesh.h"
#include "src/renderer/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "src/core/resource/resource_manager.h"
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <vector>
#include <iostream>

namespace AEngine {

bool GltfImporter::load(const std::string& path, ResourceManager& resourceManager)
{
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);

    if (result != cgltf_result_success) {
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_PARSE_FILE: " << path << std::endl;
        return false;
    }

    result = cgltf_load_buffers(&options, data, path.c_str());
    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_LOAD_BUFFERS: " << path << std::endl;
        return false;
    }

    std::cout << "GltfImporter: Loading file: " << path
              << " (" << data->materials_count << " materials, "
              << data->textures_count << " textures)" << std::endl;

    std::vector<std::shared_ptr<Mesh>> meshes;

    for (cgltf_size i = 0; i < data->meshes_count; ++i) {
        cgltf_mesh* mesh = &data->meshes[i];
        for (cgltf_size j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            if (primitive->type != cgltf_primitive_type_triangles) {
                continue; // ข้าม Primitive ที่ไม่ใช่สามเหลี่ยม
            }

            // --- Load Indices ---
            const cgltf_accessor* index_accessor = primitive->indices;
            std::vector<unsigned int> indices(index_accessor->count);
            for (cgltf_size k = 0; k < index_accessor->count; ++k) {
                indices[k] = cgltf_accessor_read_index(index_accessor, k);
            }

            // --- Load Vertex Attributes ---
            std::vector<float> vertices;
            
            // **ส่วนที่ปรับปรุง**: สร้าง Helper lambda เพื่อลดความซ้ำซ้อนในการอ่านข้อมูล Attribute
            auto read_attribute = [&](const cgltf_accessor* accessor, int offset, int components) {
                for (cgltf_size l = 0; l < accessor->count; ++l) {
                    cgltf_accessor_read_float(accessor, l, &vertices[l * 8 + offset], components);
                }
            };

            for (cgltf_size k = 0; k < primitive->attributes_count; ++k) {
                const cgltf_attribute* attribute = &primitive->attributes[k];
                const cgltf_accessor* accessor = attribute->data;

                if (vertices.empty()) {
                    // 8 floats per vertex: pos(3), normal(3), uv(2)
                    vertices.resize(accessor->count * 8, 0.0f);
                }

                if (attribute->type == cgltf_attribute_type_position) {
                    read_attribute(accessor, 0, 3); // Offset 0, 3 components
                } else if (attribute->type == cgltf_attribute_type_normal) {
                    read_attribute(accessor, 3, 3); // Offset 3, 3 components
                } else if (attribute->type == cgltf_attribute_type_texcoord) {
                    read_attribute(accessor, 6, 2); // Offset 6, 2 components
                }
            }

            if (vertices.empty() || indices.empty()) continue;

            std::string mesh_path = path + "_" + std::to_string(i) + "_" + std::to_string(j);
            auto new_mesh = resourceManager.load<Mesh>(mesh_path, vertices, indices);

            // --- Load Material and Textures ---
            const cgltf_material* material = primitive->material;
            if (material && material->has_pbr_metallic_roughness) {
                const cgltf_texture_view& base_color_texture = material->pbr_metallic_roughness.base_color_texture;
                if (base_color_texture.texture && base_color_texture.texture->image) {
                     const cgltf_image* image = base_color_texture.texture->image;
                     
                     // Case 1: Texture from URI (external file)
                     if (image->uri && strlen(image->uri) > 0) {
                        std::string base_path = path.substr(0, path.find_last_of("/\\") + 1);
                        std::string texture_path = base_path + image->uri;
                        new_mesh->setTexture(resourceManager.load<Texture>(texture_path));
                        std::cout << "GltfImporter: Loading linked texture: " << texture_path << std::endl;

                     // Case 2: Embedded Texture (from buffer)
                     } else if (image->buffer_view) {
                        cgltf_buffer_view* buffer_view = image->buffer_view;
                        const auto* buffer_data = static_cast<const unsigned char*>(buffer_view->buffer->data) + buffer_view->offset;
                        cgltf_size buffer_size = buffer_view->size;

                        std::string embedded_path = path + "_embedded_tex_" + std::to_string(i) + "_" + std::to_string(j);
                        new_mesh->setTexture(resourceManager.load<Texture>(embedded_path, buffer_data, buffer_size));
                        std::cout << "GltfImporter: Loading embedded texture from buffer." << std::endl;
                    }
                }
            }
            meshes.push_back(new_mesh);
        }
    }

    std::cout << "GltfImporter: Processed " << meshes.size() << " meshes from file: " << path << std::endl;
    cgltf_free(data);

    if (!meshes.empty()) {
        // ลงทะเบียน Mesh ชิ้นแรกด้วย path ของไฟล์ glb/gltf ดั้งเดิม
        // เพื่อให้การเรียก ResourceManager::load(path) ครั้งแรกหาเจอง่าย
        resourceManager.add(path, meshes[0]);
    }

    return !meshes.empty();
}

} // namespace AEngine