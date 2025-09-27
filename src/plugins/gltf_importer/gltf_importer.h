#pragma once


#include "core/resource/i_resource_importer.h"

#include <string>
#include <vector>
#include <memory>

namespace AEngine {


    class GltfImporter final : public IResourceImporter {
    public:
        GltfImporter() = default; // ไม่ต้องการ ResourceManager ใน constructor อีกต่อไป

        bool load(const std::string& path, ResourceManager& resourceManager) override;
        
        // บรรทัดนี้ก็เช่นกัน
        Resource::Type getResourceType() const override { return Resource::Type::Mesh; }
    
    private:
        
    };

} // namespace AEngine