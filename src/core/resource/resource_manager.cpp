#include "resource_manager.h"
#include "engine/engine.h"
#include <iostream>

// ไม่จำเป็นต้อง include สองบรรทัดนี้อีกต่อไป เพราะมันถูก include ใน resource_manager.h แล้ว
// #include "core/resource/resource.h"
// #include "plugins/gltf_importer/gltf_importer.h"

namespace AEngine {

ResourceManager::ResourceManager(Engine& context)
    : ISubsystem(context, "ResourceManager") {
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::init() {
    std::cout << "ResourceManager is initializing..." << std::endl;
    return true;
}

void ResourceManager::update(float dt) {
    // ไม่มีการทำงานใน update loop
}

void ResourceManager::shutdown() {
    std::cout << "ResourceManager is shutting down." << std::endl;
    m_resources.clear();
    m_importers.clear();
}

void ResourceManager::registerImporter(const std::vector<std::string>& extensions, IResourceImporter* importer) {
    for (const std::string& ext : extensions) {
        m_importers[ext] = importer;
    }
}

void ResourceManager::add(const std::string& path, std::shared_ptr<Resource> resource) {
    m_resources.emplace(path, resource);
}

} // namespace AEngine