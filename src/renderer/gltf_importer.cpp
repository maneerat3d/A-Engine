#include "gltf_importer.h"
#include "mesh.h"
#include "core/resource/resource_manager.h"
#include <cgltf.h>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace AEngine {

std::vector<std::shared_ptr<Mesh>> GltfImporter::load(const std::string& path, ResourceManager& resourceManager) {
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path.c_str(), &data);

    if (result != cgltf_result_success) {
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_LOAD: " << path << std::endl;
        return {};
    }

    result = cgltf_load_buffers(&options, data, path.c_str());
    if (result != cgltf_result_success) {
        cgltf_free(data);
        std::cerr << "ERROR::GLTF_IMPORTER::COULD_NOT_LOAD_BUFFERS: " << path << std::endl;
        return {};
    }

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
                meshes.push_back(resourceManager.load<Mesh>(mesh_path, vertices, indices));
            }
        }
    }

    cgltf_free(data);
    return meshes;
}

} // namespace AEngine