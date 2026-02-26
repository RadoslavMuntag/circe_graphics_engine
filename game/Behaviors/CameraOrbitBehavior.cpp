#include "CameraOrbitBehavior.h"

#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <cmath>
#include <stdexcept>

namespace Game {

CameraOrbitBehavior::CameraOrbitBehavior(float distance, float sensitivity)
    : m_OrbitDistance(distance), m_OrbitSensitivity(sensitivity) {}

Circe::CameraEntity* CameraOrbitBehavior::GetOwner() const {
    return Behavior::GetOwnerAs<Circe::CameraEntity>();
}

void CameraOrbitBehavior::OnInit() {
    auto cameraEntity = GetOwner();
    if (!cameraEntity) {
        throw std::runtime_error("CameraOrbitBehavior must be attached to a Circe::CameraEntity owner.");
    }

    Camera * camera = cameraEntity->GetCamera();
    if (!camera) {
        throw std::runtime_error("CameraOrbitBehavior requires the CameraEntity to have a valid Camera.");
    }
    
    glm::vec3 offset = camera->GetPosition() - camera->GetTarget();

    float distance = glm::length(offset);
    if (distance > 0.001f) {
        m_OrbitDistance = distance;
        m_OrbitYawDegrees = glm::degrees(std::atan2(offset.z, offset.x));
        m_OrbitPitchDegrees = glm::degrees(std::asin(offset.y / distance));
    }
    CIRCE_LOG_INFO("CameraOrbitBehavior initialized with distance: " + std::to_string(m_OrbitDistance) +
                    ", yaw: " + std::to_string(m_OrbitYawDegrees) +
                    ", pitch: " + std::to_string(m_OrbitPitchDegrees));
    camera->SetLookAt(camera->GetTarget(), camera->GetUp());

    UpdateCameraOrbit();
}

void CameraOrbitBehavior::OnEvent(Circe::Event& event) {
    Circe::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Circe::MouseButtonPressedEvent>([this](Circe::MouseButtonPressedEvent& e) {
        if (e.GetMouseButton() == 2) {
            m_IsOrbiting = true;
            m_HasLastMouse = false;
            return true;
        }
        return false;
    });

    dispatcher.Dispatch<Circe::MouseButtonReleasedEvent>([this](Circe::MouseButtonReleasedEvent& e) {
        if (e.GetMouseButton() == 2) {
            m_IsOrbiting = false;
            m_HasLastMouse = false;
            return true;
        }
        return false;
    });

    dispatcher.Dispatch<Circe::MouseMovedEvent>([this](Circe::MouseMovedEvent& e) {
        if (!m_IsOrbiting) return false;

        if (!m_HasLastMouse) {
            m_LastMouseX = e.GetX();
            m_LastMouseY = e.GetY();
            m_HasLastMouse = true;
            return true;
        }

        float dx = e.GetX() - m_LastMouseX;
        float dy = e.GetY() - m_LastMouseY;
        m_LastMouseX = e.GetX();
        m_LastMouseY = e.GetY();

        m_OrbitYawDegrees += dx * m_OrbitSensitivity;
        m_OrbitPitchDegrees += dy * m_OrbitSensitivity;
        m_OrbitPitchDegrees = glm::clamp(m_OrbitPitchDegrees, -85.0f, 85.0f);

        UpdateCameraOrbit();
        return true;
    });

    dispatcher.Dispatch<Circe::MouseScrolledEvent>([this](Circe::MouseScrolledEvent& e) {
        m_OrbitDistance -= e.GetYOffset() * m_ZoomSpeed;
        m_OrbitDistance = glm::clamp(m_OrbitDistance, m_MinOrbitDistance, m_MaxOrbitDistance);
        UpdateCameraOrbit();
        return true;
    });
}

void CameraOrbitBehavior::UpdateCameraOrbit() {
    auto* owner = GetOwner();
    if (!owner) return;

    Circe::Camera* camera = owner->GetCamera();
    if (!camera) return;

    float yawRad = glm::radians(m_OrbitYawDegrees);
    float pitchRad = glm::radians(m_OrbitPitchDegrees);

    glm::vec3 offset;
    offset.x = m_OrbitDistance * std::cos(pitchRad) * std::cos(yawRad);
    offset.y = m_OrbitDistance * std::sin(pitchRad);
    offset.z = m_OrbitDistance * std::cos(pitchRad) * std::sin(yawRad);

    camera->SetPosition(camera->GetTarget() + offset);
    camera->SetLookAt(camera->GetTarget(), camera->GetUp());
}

}