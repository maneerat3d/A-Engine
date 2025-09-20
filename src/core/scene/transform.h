#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AEngine {

struct Transform {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat rotation = {1.0f, 0.0f, 0.0f, 0.0f}; // W, X, Y, Z
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    glm::mat4 getMatrix() const {
        glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return posMatrix * rotMatrix * scaleMatrix;
    }
};

} // namespace AEngine