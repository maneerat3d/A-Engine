#include "game_plugin.h"
#include "engine/engine.h"
#include "game_system.h"
#include <iostream>

namespace AEngine {

GamePlugin::GamePlugin(Engine& engine) : m_engine(engine) {
    std::cout << "Game Plugin Created" << std::endl;
}

GamePlugin::~GamePlugin() {
    std::cout << "Game Plugin Destroyed" << std::endl;
}

void GamePlugin::createSystems(Engine& engine) {
    engine.addSystem(new GameSystem());
}

void GamePlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine