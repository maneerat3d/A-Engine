#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "resource.h"

// Forward declare
namespace AEngine {
    class IResourceImporter; 


class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager();

    void registerImporter(const std::vector<std::string>& extensions, AEngine::IResourceImporter* importer);


    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path, Args&&... args) {
        if (m_resources.find(path) != m_resources.end()) {
            return std::static_pointer_cast<T>(m_resources[path]);
        }
        // สร้าง Resource ใหม่ถ้ายังไม่เคยโหลด
        auto resource = std::make_shared<T>(*this, path, std::forward<Args>(args)...);
        m_resources[path] = resource;
        return resource;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<AEngine::Resource>> m_resources; // <--- เพิ่ม AEngine::

    std::unordered_map<std::string, AEngine::IResourceImporter*> m_importers;
};

} // namespace AEngine