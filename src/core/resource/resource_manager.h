#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace AEngine {
class Resource; // Forward declaration

class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager();

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
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
};

} // namespace AEngine