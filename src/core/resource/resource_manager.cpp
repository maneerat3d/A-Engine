#pragma once
#include "resource_manager.h"
#include "plugins/gltf_importer/gltf_importer.h"
#include <iostream> // เพิ่มเข้ามาเพื่อ Logging
namespace AEngine {
// --- เพิ่ม Constructor ---
ResourceManager::ResourceManager(Engine& context)
    : ISubsystem(context, "ResourceManager")
{
}

ResourceManager::~ResourceManager() {
    m_resources.clear();
}

// --- เพิ่ม Implementation ของ ISubsystem ---
bool ResourceManager::init() {
    std::cout << "ResourceManager is initializing..." << std::endl;
    // ปัจจุบันยังไม่มีอะไรต้องทำตอน init
    return true;
}

void ResourceManager::update(float dt) {
    // ปัจจุบัน ResourceManager ไม่จำเป็นต้องอัปเดตทุกเฟรม
}

void ResourceManager::shutdown() {
    std::cout << "ResourceManager is shutting down." << std::endl;
    m_resources.clear();
}


void ResourceManager::registerImporter(const std::vector<std::string>& extensions, IResourceImporter* importer) {
    for (const auto& ext : extensions) {
        m_importers[ext] = importer;
    }
}

} // namespace AEngine