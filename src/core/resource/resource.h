#pragma once
#include <string>

namespace AEngine {
class ResourceManager;

class Resource {
public:
    enum class Type // <--- enum ต้องอยู่ข้างใน class
    {
        Texture,
        Mesh,
        Sound
    };
    Resource(ResourceManager& owner, const std::string& path)
        : m_owner(owner), m_path(path) {}
    virtual ~Resource() = default;

    const std::string& getPath() const { return m_path; }

protected:
    ResourceManager& m_owner;
    std::string m_path;
};

} // namespace AEngine