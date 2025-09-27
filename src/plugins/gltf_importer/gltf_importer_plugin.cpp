#include "gltf_importer_plugin.h"
#include "gltf_importer.h"
#include "engine/engine.h"
#include "core/resource/resource_manager.h"
#include "core/memory/unique_ptr.h"
#include <iostream>

namespace AEngine {
    // PImpl (Pointer to implementation) idiom
    // ซ่อนรายละเอียดการ implement ไว้ในไฟล์ .cpp
    struct GltfImporterPluginImpl {
        // **ส่วนที่ปรับปรุง**: เปลี่ยนจาก Array<UniquePtr<...>> มาเป็น UniquePtr ตัวเดียว
        // เพราะ Plugin นี้จัดการ Importer แค่ประเภทเดียว ทำให้โค้ดง่ายลง
        UniquePtr<IResourceImporter> m_importer;
    };

    GltfImporterPlugin::GltfImporterPlugin(Engine& engine)
        : m_engine(engine)
    {
        // สร้าง implementation object
        m_pimpl = AENGINE_NEW(engine.getAllocator(), GltfImporterPluginImpl)();
    }

    GltfImporterPlugin::~GltfImporterPlugin() {
        // ทำลาย implementation object เพื่อป้องกัน memory leak
        AENGINE_DELETE(m_engine.getAllocator(), m_pimpl);
    }

    void GltfImporterPlugin::createSystems(Engine& engine) {
        std::cout << "GltfImporterPlugin: Creating and registering importer..." << std::endl;
        
        // **ส่วนที่ปรับปรุง**: สร้าง Importer และให้ PImpl จัดการ Ownership โดยตรง
        m_pimpl->m_importer = UniquePtr<GltfImporter>::create(engine.getAllocator());

        ResourceManager* manager = engine.getSubsystem<ResourceManager>();
        if (manager) {
            // ลงทะเบียน Importer โดยส่ง Raw Pointer ไปให้ Manager
            // Ownership ของ Importer ยังคงอยู่ที่ Plugin (m_pimpl)
            manager->registerImporter({".gltf", ".glb"}, m_pimpl->m_importer.get());
        }
    }
 
    extern "C" __declspec(dllexport) IPlugin* createPlugin(Engine& engine) {
        return new GltfImporterPlugin(engine);
    }
} // namespace AEngine