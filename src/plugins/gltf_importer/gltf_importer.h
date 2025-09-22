#pragma once

// CRITICAL: นี่คือการนำ "CV/Resume" ของ Resource เข้ามา
// ทำให้ Compiler รู้จักทุกอย่างที่อยู่ข้างใน รวมถึง `enum class Type`
#include "core/resource/resource.h"

#include <string>
#include <vector>
#include <memory>

namespace AEngine {

    // เราสามารถใช้ Forward Declaration กับ ResourceManager ได้
    // เพราะใน header นี้เราใช้มันเป็นแค่ Reference (&) เท่านั้น
    class ResourceManager;

    // Interface กลางสำหรับ Importer ทุกประเภท
    class IResourceImporter {
    public:
        virtual ~IResourceImporter() = default;
        virtual bool load(const std::string& path, ResourceManager& resourceManager) = 0;
        
        // บรรทัดนี้ต้องการ Full Definition ของ Resource เพื่อให้รู้จัก enum Type
        virtual Resource::Type getResourceType() const = 0; 
    };

    class GltfImporter final : public IResourceImporter {
    public:
        explicit GltfImporter(ResourceManager& resourceManager);

        bool load(const std::string& path, ResourceManager& resourceManager) override;
        
        // บรรทัดนี้ก็เช่นกัน
        Resource::Type getResourceType() const override { return Resource::Type::Mesh; }
    
    private:
        ResourceManager& m_resourceManager;
    };

} // namespace AEngine