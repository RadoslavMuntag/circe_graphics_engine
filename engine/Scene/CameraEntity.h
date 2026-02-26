#pragma once

#include "Entity.h"
#include "../Renderer/Camera.h"
#include <memory>

namespace Circe {
    // Entity wrapper that owns or references a Camera
    class CIRCE_API CameraEntity : public Entity {
    public:
        CameraEntity(const std::string& name, std::shared_ptr<Camera> camera)
            : Entity(name), m_Camera(camera) {}
        
        Camera* GetCamera() const { return m_Camera.get(); }
        std::shared_ptr<Camera> GetCameraShared() const { return m_Camera; }
        
    private:
        std::shared_ptr<Camera> m_Camera;
    };
}