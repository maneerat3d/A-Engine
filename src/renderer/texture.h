#pragma once

#include "core/resource/resource.h"

namespace AEngine {

class Texture : public Resource {
public:
    Texture(ResourceManager& owner, const std::string& path);
    Texture(ResourceManager& owner, const std::string& path, const unsigned char* data, size_t size);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    unsigned int getRendererID() const { return m_rendererID; }

private:
    unsigned int m_rendererID = 0;
    int m_width = 0, m_height = 0, m_bpp = 0;
};

} // namespace AEngine