#include "render_system.h"
#include "renderer.h"

namespace AEngine {

RenderSystem::RenderSystem(SDL_Window* window)
    : m_renderer(nullptr), m_window(window) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::init() {
    m_renderer = new Renderer();
    m_renderer->init(m_window);
}

void RenderSystem::update(Scene& scene, float dt) {
    m_renderer->render(scene);
}

void RenderSystem::shutdown() {
    if (m_renderer) {
        m_renderer->shutdown();
        delete m_renderer;
        m_renderer = nullptr;
    }
}

} // namespace AEngine