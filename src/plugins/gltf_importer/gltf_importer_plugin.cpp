#include "gltf_importer_plugin.h"
#include "gltf_importer.h"
#include "engine/engine.h"
#include "core/resource/resource_manager.h"
#include "core/container/array.h"
#include "core/memory/unique_ptr.h"
#include <iostream>

namespace AEngine {
    // PImpl (Pointer to implementation) idiom
    // struct นี้จะถูกซ่อนไว้ในไฟล์ .cpp เท่านั้น
    struct GltfImporterPluginImpl {
        Array<UniquePtr<IResourceImporter>> m_importer;
        GltfImporterPluginImpl(IAllocator& allocator) : m_importer(allocator) {}
    };
}

namespace AEngine {

    GltfImporterPlugin::GltfImporterPlugin(Engine& engine)
        : m_engine(engine)
    {
        // สร้าง implementation object ใน constructor
        m_pimpl = AENGINE_NEW(engine.getAllocator(), GltfImporterPluginImpl)(engine.getAllocator());
    }

    GltfImporterPlugin::~GltfImporterPlugin() {
        // ทำลาย implementation object ใน destructor เพื่อป้องกัน memory leak
        AENGINE_DELETE(m_engine.getAllocator(), m_pimpl);
    }

    void GltfImporterPlugin::createSystems(Engine& engine) {
         std::cout << "GltfImporterPlugin CREATED and registering importer..." << std::endl;
        auto gltf_importer = UniquePtr<GltfImporter>::create(engine.getAllocator());

        ResourceManager* manager = engine.getSubsystem<ResourceManager>();
        if (manager) {
            manager->registerImporter({".gltf", ".glb"}, gltf_importer.get());
        }

        // เข้าถึง member ผ่าน pointer
        m_pimpl->m_importer.push(std::move(gltf_importer));
    }
 
    extern "C" __declspec(dllexport) IPlugin* createPlugin(Engine& engine) {
        return new GltfImporterPlugin(engine);
    }
} // namespace AEngine