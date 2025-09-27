#pragma once
#include "core/plugin/iplugin.h"
#include "core/resource/i_resource_importer.h"

namespace AEngine {
    class Engine;

    // คลาสเดียวที่สืบทอดจาก Interface ทั้งสอง
    class GltfImporterPlugin final : public IPlugin, public IResourceImporter {
    public:
        explicit GltfImporterPlugin(Engine& engine);
        ~GltfImporterPlugin() override;

        // --- IPlugin Implementation ---
        const char* getName() const override { return "GltfImporterPlugin"; }
        void createSystems(Engine& engine) override;
        void destroySystems(Engine& engine) override;
        
        // --- IResourceImporter Implementation ---
        bool load(const std::string& path, ResourceManager& resourceManager) override;
        Resource::Type getResourceType() const override { return Resource::Type::Mesh; }
        
    private:
        Engine& m_engine;
    };

} // namespace AEngine