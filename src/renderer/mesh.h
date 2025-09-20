#pragma once
#include "resource/resource.h"
#include <vector>

namespace AEngine {

class Mesh : public Resource {
public:
    // ตอนนี้เราจะสร้าง Mesh จาก vertex data โดยตรง
    // ในอนาคตเราจะเปลี่ยนให้โหลดจากไฟล์ .obj หรือ .fbx
    Mesh(ResourceManager& owner, const std::string& path, const std::vector<float>& vertices);
    ~Mesh();
    
    void bind() const;
    void unbind() const;

    unsigned int getVertexCount() const { return m_vertexCount; }

private:
    unsigned int m_vao = 0, m_vbo = 0;
    unsigned int m_vertexCount = 0;
};

} // namespace AEngine