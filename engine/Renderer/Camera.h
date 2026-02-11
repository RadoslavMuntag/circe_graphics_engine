#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Circe {

    class Camera {
    public:
        Camera(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
        ~Camera() = default;

        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void SetLookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
        void SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

        glm::vec3 GetPosition() const { return m_Position; }
        glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

        glm::vec3 m_Position;
        glm::vec3 m_Target;
        glm::vec3 m_Up;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
    };

}
