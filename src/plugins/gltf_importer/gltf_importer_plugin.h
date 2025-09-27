#pragma once
#include "core/plugin/iplugin.h"

namespace AEngine {
    class Engine;
    
    // Forward declare แค่ struct ที่เป็น implementation ก็พอ
    struct GltfImporterPluginImpl;

    class GltfImporterPlugin final : public IPlugin {
    public:
        explicit GltfImporterPlugin(Engine& engine);
        ~GltfImporterPlugin() override;

        const char* getName() const override { return "gltf_importer"; }

        void createSystems(Engine& engine) override;
        void destroySystems(Engine& engine) override { /* ไม่ต้องทำอะไร */ }
        
    private:
        Engine& m_engine;
        GltfImporterPluginImpl* m_pimpl; // Pointer to implementation
    };

} // namespace AEngine