#pragma once
#include "core/core.h"

namespace AEngine {

class Framebuffer {
public:
    Framebuffer(u32 width, u32 height);
    ~Framebuffer();

    void bind();
    void unbind();
    void resize(u32 width, u32 height);

    u32 getColorAttachmentID() const { return m_color_attachment; }
    u32 getWidth() const { return m_width; }
    u32 getHeight() const { return m_height; }

private:
    void invalidate();

    u32 m_renderer_id = 0;
    u32 m_color_attachment = 0;
    u32 m_depth_attachment = 0;
    u32 m_width, m_height;
};

} // namespace AEngine