#pragma once
#include <string>
#include "resource.h"

namespace AEngine {    
    class ResourceManager;

    // Interface กลางสำหรับ Importer ทุกประเภท
    // **หลักการ**: Interface นี้เปรียบเสมือน "สัญญา" ที่ Importer ทุกตัวต้องทำให้ได้
    class IResourceImporter {
    public:
        virtual ~IResourceImporter() = default;
        virtual bool load(const std::string& path, ResourceManager& resourceManager) = 0;
        
        // บรรทัดนี้ต้องการ Full Definition ของ Resource เพื่อให้รู้จัก enum Type
        virtual Resource::Type getResourceType() const = 0; 
    };
}  // namespace AEngine