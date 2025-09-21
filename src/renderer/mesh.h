#pragma once
#include "resource/resource.h"
#include <vector>

namespace AEngine {

class Mesh : public Resource {
public:
    // Constructor ใหม่ที่รองรับทั้ง Vertices และ Indices
    Mesh(ResourceManager& owner, const std::string& path, 
         const std::vector<float>& vertices, 
         const std::vector<unsigned int>& indices);
    ~Mesh();
    
    void bind() const;
    void unbind() const;

    // เราจะใช้จำนวน Index ในการวาดแทนจำนวน Vertex
    unsigned int getIndexCount() const { return m_indexCount; }

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0; // Vertex Buffer Object
    unsigned int m_ebo = 0; // Element Buffer Object (for indices)
    unsigned int m_indexCount = 0;
};

} // namespace AEngine