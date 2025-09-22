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

    void GltfImporterPlugin::create() {
         std::cout << "GltfImporterPlugin CREATED and registering importer..." << std::endl;

        // สร้าง Importer ของเรา
         auto gltf_importer = std::make_unique<GltfImporter>(*m_engine.getResourceManager());

        // ลงทะเบียน Importer กับ ResourceManager
        ResourceManager& manager = *m_engine.getResourceManager(); // Dereference อีกครั้ง
        manager.registerImporter({".gltf", ".glb"}, gltf_importer.get());

        // เก็บ Importer ไว้เพื่อจัดการ lifetime
        m_importer.push_back(std::move(gltf_importer));
    }

    void GltfImporterPlugin::destroy() {
        std::cout << "GltfImporterPlugin DESTROYED" << std::endl;
        
        // ควรจะ unregister ด้วย แต่ตอนนี้เอาแค่ clear memory ก่อน
        m_importer.clear();
    }

 

    // ฟังก์ชันนี้สำคัญมาก! Engine จะเรียกใช้เพื่อสร้าง Plugin ของเรา
    // ตรวจสอบให้แน่ใจว่ามีฟังก์ชันนี้อยู่
    extern "C" __declspec(dllexport) IPlugin* createPlugin(Engine& engine) {
        return new GltfImporterPlugin(engine);
    }
} // namespace AEngine