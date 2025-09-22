#pragma once
#include "core/plugin/iplugin.h"
#include <memory>
#include <vector>

namespace AEngine {
    class Engine;
    class IResourceImporter;

    class GltfImporterPlugin final : public IPlugin {
    public:
        explicit GltfImporterPlugin(Engine& engine);
        ~GltfImporterPlugin() override;

        // --- IPlugin Interface (ฉบับแก้ไขให้ถูกต้อง) ---
        const char* getName() const override { return "gltf_importer"; }

        // GltfImporterPlugin ไม่ได้สร้าง System ของตัวเอง เลยใส่เป็นฟังก์ชันว่างๆ ไว้
        void createSystems(Engine& engine) override { /* ทำตามสัญญา แต่ไม่ต้องทำอะไร */ }
        void destroySystems(Engine& engine) override { /* ทำตามสัญญา แต่ไม่ต้องทำอะไร */ }
        
        // เราจะสร้างฟังก์ชัน create และ destroy ของตัวเองขึ้นมาใหม่ โดยไม่ใช้ override
        // เพราะมันเป็นฟังก์ชันเฉพาะของ Plugin นี้ ไม่ได้มาจาก Interface
        void create();
        void destroy();
        
    private:
        Engine& m_engine;
        std::vector<std::unique_ptr<IResourceImporter>> m_importer;
    };

} // namespace AEngine