#include "game_system.h"
#include "core/scene/scene.h"
#include <SDL.h>
#include <glm/gtc/quaternion.hpp>

namespace AEngine {

void GameSystem::init() {
    // Initialization logic for game systems if needed
}

void GameSystem::update(Scene& scene, float dt) {
    // Logic for rotating cubes, moved from engine.cpp
    float time = (float)SDL_GetTicks() / 1000.0f;
    for (Entity i = 1; i < scene.getEntityCount(); ++i) {
        // This is still a simplified way to iterate. A real system would be more efficient.
        // For example, it would only iterate over entities that have both components.
        auto& rot_comp = scene.getComponent<ECS::RotatingCubeComponent>(i);
        if (rot_comp.rotationSpeed > 0.0f) { // Check if the component is active
             auto& transform = scene.getComponent<ECS::TransformComponent>(i);
             transform.rotation = glm::angleAxis(time * rot_comp.rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));
        }
    }
}

void GameSystem::shutdown() {
    // Shutdown logic for game systems if needed
}

} // namespace AEngine