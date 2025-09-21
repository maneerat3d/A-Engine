#include "game_plugin.h"
#include "engine.h"
#include "game_system.h"

namespace AEngine {

void GamePlugin::createSystems(Engine& engine) {
    engine.addSystem(new GameSystem());
}

void GamePlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine