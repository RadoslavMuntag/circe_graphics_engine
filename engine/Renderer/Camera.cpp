#include "pch.h"

#include "Camera.h"

namespace Circe {

    Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
        : m_Position(0.0f, 0.0f, 5.0f),
          m_Target(0.0f, 0.0f, 0.0f),
          m_Up(0.0f, 1.0f, 0.0f) {
        SetPerspective(fov, aspectRatio, nearPlane, farPlane);
        RecalculateViewMatrix();
    }

    void Camera::SetLookAt(const glm::vec3& target, const glm::vec3& up) {
        m_Target = target;
        m_Up = up;
        RecalculateViewMatrix();
    }

    void Camera::SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void Camera::RecalculateViewMatrix() {
        m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
    }

}
