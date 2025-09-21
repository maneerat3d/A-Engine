#include "renderer_plugin.h"
#include "engine/engine.h"
#include "render_system.h"

namespace AEngine {

void RendererPlugin::createSystems(Engine& engine) {
    engine.addSystem(new RenderSystem(engine.getWindow()));
}

void RendererPlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine