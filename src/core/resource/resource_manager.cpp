#include "resource_manager.h"
#include "plugins/gltf_importer/gltf_importer.h" // ต้อง include IResourceImporter


namespace AEngine {

ResourceManager::~ResourceManager() {
    m_resources.clear();
}

void ResourceManager::registerImporter(const std::vector<std::string>& extensions, IResourceImporter* importer) {
    for (const auto& ext : extensions) {
        m_importers[ext] = importer;
    }
}

} // namespace AEngine