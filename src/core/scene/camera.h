#pragma once

#include <glm/glm.hpp>

namespace AEngine {

class Camera {
public:
    Camera();

    void setPerspective(float fov, float aspect, float near, float far);

    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
    const glm::vec3& getPosition() const { return m_position; }

    void setPosition(const glm::vec3& position);
    void lookAt(const glm::vec3& target, const glm::vec3& up = {0.0f, 1.0f, 0.0f});

private:
    void updateViewMatrix();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};

} // namespace AEngine