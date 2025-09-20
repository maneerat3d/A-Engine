#include "scene.h"

namespace AEngine {

Scene::Scene() {
}

Scene::~Scene() {
}

GameObject& Scene::createGameObject(const std::string& name) {
    m_gameObjects.emplace_back();
    m_gameObjects.back().name = name;
    return m_gameObjects.back();
}

} // namespace AEngine