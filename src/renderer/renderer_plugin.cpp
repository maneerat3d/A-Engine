#include "renderer_plugin.h"
#include "engine/engine.h"
#include "render_system.h"
#include "core/memory/core.h" // <--- เพิ่มบรรทัดนี้
#include <iostream>

namespace AEngine {

RendererPlugin::RendererPlugin(Engine& engine) : m_engine(engine) {
    std::cout << "Renderer Plugin Created" << std::endl;
}

void RendererPlugin::createSystems(Engine& engine) {
    // ส่ง engine context เข้าไปให้ RenderSystem โดยตรง และใช้ Allocator ของ Engine
    engine.addSystem(AENGINE_NEW(engine.getAllocator(), RenderSystem)(engine));
}

void RendererPlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine