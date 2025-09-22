// src/plugins/gltf_importer/gltf_importer_plugin.cpp

#include "gltf_importer_plugin.h"
#include "gltf_importer.h"
#include "engine/engine.h"
#include "core/resource/resource_manager.h"
#include <iostream>

namespace AEngine {

    GltfImporterPlugin::GltfImporterPlugin(Engine& engine)
        : m_engine(engine) {}

    GltfImporterPlugin::~GltfImporterPlugin() {}

    void GltfImporterPlugin::createSystems(Engine& engine) {
         std::cout << "GltfImporterPlugin CREATED and registering importer..." << std::endl;
        auto gltf_importer = std::make_unique<GltfImporter>();

        // --- การเปลี่ยนแปลงสำคัญ ---
        // ขอ ResourceManager จาก Engine Context
        ResourceManager* manager = engine.getSubsystem<ResourceManager>();
        if (manager) {
            manager->registerImporter({".gltf", ".glb"}, gltf_importer.get());
        }
        // -------------------------

        m_importer.push_back(std::move(gltf_importer));
    }
 

    // ฟังก์ชันนี้สำคัญมาก! Engine จะเรียกใช้เพื่อสร้าง Plugin ของเรา
    // ตรวจสอบให้แน่ใจว่ามีฟังก์ชันนี้อยู่
    extern "C" __declspec(dllexport) IPlugin* createPlugin(Engine& engine) {
        return new GltfImporterPlugin(engine);
    }
} // namespace AEngine