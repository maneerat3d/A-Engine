#pragma once

#include "camera.h"
#include "game_object.h"
#include <vector>
#include <memory>

namespace AEngine {

class Scene {
public:
    Scene();
    ~Scene();

    GameObject& createGameObject(const std::string& name = "GameObject");

    const std::vector<GameObject>& getGameObjects() const { return m_gameObjects; }
    Camera& getCamera() { return m_camera; }

private:
    std::vector<GameObject> m_gameObjects;
    Camera m_camera;
};

} // namespace AEngine