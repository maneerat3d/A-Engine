// A-Engine/src/renderer/framebuffer.cpp

#include "framebuffer.h"
#include <glad/glad.h>
#include <iostream>

namespace AEngine {

Framebuffer::Framebuffer(u32 width, u32 height)
    : m_width(width), m_height(height) {
    invalidate();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &m_renderer_id);
    glDeleteTextures(1, &m_color_attachment);
    glDeleteTextures(1, &m_depth_attachment);
}

void Framebuffer::invalidate() {
    if (m_renderer_id) {
        glDeleteFramebuffers(1, &m_renderer_id);
        glDeleteTextures(1, &m_color_attachment);
        glDeleteTextures(1, &m_depth_attachment);
    }

    // --- ส่วนที่แก้ไข: ใช้ glGen* แทน glCreate* ---
    glGenFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    // Color attachment
    glGenTextures(1, &m_color_attachment);
    glBindTexture(GL_TEXTURE_2D, m_color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

    // Depth-stencil attachment
    glGenTextures(1, &m_depth_attachment);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
    // --- ส่วนที่แก้ไข: ใช้ glTexImage2D แทน glTexStorage2D ---
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glViewport(0, 0, m_width, m_height);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(u32 width, u32 height) {
    if (width == 0 || height == 0 || (width == m_width && height == m_height)) {
        return;
    }
    m_width = width;
    m_height = height;
    invalidate();
}

} // namespace AEngine