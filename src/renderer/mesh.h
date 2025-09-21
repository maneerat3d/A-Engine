#pragma once

#include "core/resource/resource.h"
#include <vector>
#include <memory>

namespace AEngine {

class ResourceManager; // Forward declaration
class Texture;

class Mesh : public Resource {
public:
    // แก้ไขบรรทัดนี้ให้ถูกต้อง
    Mesh(ResourceManager& owner, const std::string& path, const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void bind() const;
    void unbind() const;
    unsigned int getIndexCount() const;

    void setTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> getTexture() const;

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    unsigned int m_indexCount;
    std::shared_ptr<Texture> m_texture;
};

} // namespace AEngine