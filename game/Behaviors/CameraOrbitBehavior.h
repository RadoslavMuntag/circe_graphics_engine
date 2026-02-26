#pragma once

#include <Circe.h>

namespace Game {

    class CameraOrbitBehavior : public Circe::Behavior {
    public:
        CameraOrbitBehavior(float distance = 3.0f, float sensitivity = 0.2f);

        Circe::CameraEntity* GetOwner() const;
        void OnInit() override;
        void OnEvent(Circe::Event& event) override;

    private:
        void UpdateCameraOrbit();

        bool m_IsOrbiting = false;
        bool m_HasLastMouse = false;
        float m_LastMouseX = 0.0f;
        float m_LastMouseY = 0.0f;
        float m_OrbitDistance = 3.0f;
        float m_OrbitYawDegrees = -90.0f;
        float m_OrbitPitchDegrees = 0.0f;
        float m_OrbitSensitivity = 0.2f;
        float m_ZoomSpeed = 0.25f;
        float m_MinOrbitDistance = 1.0f;
        float m_MaxOrbitDistance = 30.0f;
    };

} // namespace Game