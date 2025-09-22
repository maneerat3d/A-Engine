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
#include <stdexcept>

namespace AEngine {

GltfImporter::GltfImporter(ResourceManager& resourceManager)
    : m_resourceManager(resourceManager) {}

bool GltfImporter::load(const std::string& path, ResourceManager& resourceManager)
{
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);

    if (result != cgltf_result_success) {
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_LOAD: " << path << std::endl;
        return false;
    }

    result = cgltf_load_buffers(&options, data, path.c_str());
    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_LOAD_BUFFERS: " << path << std::endl;
        return {};
    }

    // --- LOGGING START ---
    std::cout << "GLTF_IMPORTER::INFO: Loading file: " << path << std::endl;
    std::cout << "GLTF_IMPORTER::INFO: Found " << data->materials_count << " materials." << std::endl;
    std::cout << "GLTF_IMPORTER::INFO: Found " << data->textures_count << " textures." << std::endl;
    // --- LOGGING END ---

    std::vector<std::shared_ptr<Mesh>> meshes;

    for (cgltf_size i = 0; i < data->meshes_count; ++i) {
        cgltf_mesh* mesh = &data->meshes[i];
        for (cgltf_size j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            if (primitive->type != cgltf_primitive_type_triangles) {
                continue; // Skip non-triangle primitives
            }

            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            // Load Indices
            const cgltf_accessor* index_accessor = primitive->indices;
            indices.resize(index_accessor->count);
            for (cgltf_size k = 0; k < index_accessor->count; ++k) {
                indices[k] = cgltf_accessor_read_index(index_accessor, k);
            }

            // Load Vertices
            for (cgltf_size k = 0; k < primitive->attributes_count; ++k) {
                cgltf_attribute* attribute = &primitive->attributes[k];
                const cgltf_accessor* accessor = attribute->data;

                if (vertices.empty()) {
                    vertices.resize(accessor->count * 8, 0.0f); // 8 floats: pos, normal, uv
                }

                if (attribute->type == cgltf_attribute_type_position) {
                    for (cgltf_size l = 0; l < accessor->count; ++l) {
                        cgltf_accessor_read_float(accessor, l, &vertices[l * 8], 3);
                    }
                } else if (attribute->type == cgltf_attribute_type_normal) {
                    for (cgltf_size l = 0; l < accessor->count; ++l) {
                        cgltf_accessor_read_float(accessor, l, &vertices[l * 8 + 3], 3);
                    }
                } else if (attribute->type == cgltf_attribute_type_texcoord) {
                    for (cgltf_size l = 0; l < accessor->count; ++l) {
                         cgltf_accessor_read_float(accessor, l, &vertices[l * 8 + 6], 2);
                    }
                }
            }
            
            if (!vertices.empty() && !indices.empty()) {
                std::string mesh_path = path + "_" + std::to_string(i) + "_" + std::to_string(j);
                auto new_mesh = resourceManager.load<Mesh>(mesh_path, vertices, indices);

                const cgltf_material* material = primitive->material;
                if (material && material->has_pbr_metallic_roughness) {
                    // --- LOGGING START ---
                    std::cout << "GLTF_IMPORTER::DEBUG: Processing material '" << (material->name ? material->name : "N/A") << "' for mesh " << i << ", primitive " << j << std::endl;
                    // --- LOGGING END ---                    
                    const cgltf_texture_view& base_color_texture = material->pbr_metallic_roughness.base_color_texture;
                    if (base_color_texture.texture && base_color_texture.texture->image) {
                        const char* uri = base_color_texture.texture->image->uri;
                        if (uri && strlen(uri) > 0) {
                            std::string base_path = path.substr(0, path.find_last_of("/\\") + 1);
                            std::string texture_path = base_path + uri;
                            std::cout << "GLTF_IMPORTER::DEBUG: Found texture URI, loading: " << texture_path << std::endl;
                            new_mesh->setTexture(resourceManager.load<Texture>(texture_path));
                            
                         } else if (base_color_texture.texture->image->buffer_view) {
                            // --- จัดการกับ Embedded Texture ---
                            cgltf_buffer_view* buffer_view = base_color_texture.texture->image->buffer_view;
                            const unsigned char* buffer_data = static_cast<const unsigned char*>(buffer_view->buffer->data) + buffer_view->offset;
                            cgltf_size buffer_size = buffer_view->size;

                            std::cout << "GLTF_IMPORTER::DEBUG: Found embedded texture, decoding from memory." << std::endl;
                            
                            // สร้าง Path ที่ไม่ซ้ำกันสำหรับ Resource Manager
                            std::string embedded_path = path + "_embedded_tex_" + std::to_string(i) + "_" + std::to_string(j);
                            new_mesh->setTexture(resourceManager.load<Texture>(embedded_path, buffer_data, buffer_size));
                        }
                    }
                }
                meshes.push_back(new_mesh);
            }
        }
    }
    std::cout << "GltfImporter: Successfully loaded " << path << std::endl;
    cgltf_free(data);
    return true;
}

} // namespace AEngine