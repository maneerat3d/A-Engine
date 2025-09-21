#include "texture.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

namespace AEngine {

// Constructor สำหรับโหลดจากไฟล์
Texture::Texture(ResourceManager& owner, const std::string& path)
    : Resource(owner, path) {
    
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char *pixel_data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (pixel_data) {
        m_width = width;
        m_height = height;

        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else { /* ... handle error ... */ }

        glGenTextures(1, &m_rendererID); // <--- แก้ไข
        glBindTexture(GL_TEXTURE_2D, m_rendererID); // <--- แก้ไข

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(pixel_data);
    } else {
        std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD_FROM_FILE: " << path << std::endl;
    }
}

// Constructor สำหรับโหลดจาก Memory (สำหรับ Embedded Texture)
Texture::Texture(ResourceManager& owner, const std::string& path, const unsigned char* data, size_t size)
    : Resource(owner, path) {
    
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char *pixel_data = stbi_load_from_memory(data, static_cast<int>(size), &width, &height, &nrChannels, 0);

    if (pixel_data) {
        m_width = width;
        m_height = height;

        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else { /* ... handle error ... */ }

        glGenTextures(1, &m_rendererID); // <--- แก้ไข
        glBindTexture(GL_TEXTURE_2D, m_rendererID); // <--- แก้ไข
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(pixel_data);
    } else {
        std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD_FROM_MEMORY: " << path << std::endl;
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &m_rendererID); // <--- แก้ไข
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID); // <--- แก้ไข
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace AEngine