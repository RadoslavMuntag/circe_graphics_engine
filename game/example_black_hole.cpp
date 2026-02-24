#include "Core/Log.h"
#include "Renderer/Texture.h"
#include "Scene/Behavior.h"
#include <Circe.h>

#include <glm/glm.hpp>
#include <cmath>
#include <memory>

namespace {
    class GravityBehavior : public Circe::Behavior {
    public:
        GravityBehavior(float mass = 1.0f)
            : m_Mass(mass), m_Velocity(0.0f), m_Acceleration(0.0f) {}

        void OnUpdate(float dt) override {
            if (!GetOwner()) return;

            auto& transform = GetOwner()->GetTransform();

            // Apply acceleration to velocity
            m_Velocity += m_Acceleration * dt;

            // Apply velocity to position
            transform.Position += m_Velocity * dt;

            // Reset acceleration for next frame
            m_Acceleration = glm::vec3(0.0f);
        }

        void Update(float dt) {
            if (!GetOwner()) return;

            auto& transform = GetOwner()->GetTransform();

            // Apply acceleration to velocity
            m_Velocity += m_Acceleration * dt;

            // Apply velocity to position
            transform.Position += m_Velocity * dt;

            // Reset acceleration for next frame
            m_Acceleration = glm::vec3(0.0f);
        }

        void ApplyForce(const glm::vec3& force) {
            m_Acceleration += force / m_Mass;
        }

        void SetVelocity(const glm::vec3& velocity) {
            m_Velocity = velocity;
        }

        float GetMass() const { return m_Mass; }
        glm::vec3 GetVelocity() const { return m_Velocity; }

    private:
        float m_Mass;
        glm::vec3 m_Velocity;
        glm::vec3 m_Acceleration;
    };

    class RayMarchingScene : public Circe::Scene {
    public:
        void SetCamera(Circe::Camera* camera, const glm::vec3& target) {
            m_Camera = camera;
            m_CameraTarget = target;
            if (m_Camera) {
                glm::vec3 offset = m_Camera->GetPosition() - m_CameraTarget;
                float distance = glm::length(offset);
                if (distance > 0.001f) {
                    m_OrbitDistance = distance;
                    m_OrbitYawDegrees = glm::degrees(std::atan2(offset.z, offset.x));
                    m_OrbitPitchDegrees = glm::degrees(std::asin(offset.y / distance));
                }
                m_Camera->SetLookAt(m_CameraTarget, m_CameraUp);
            }
        }

        RayMarchingScene() {
            m_QuadMesh = Circe::GenerateQuadMesh(2.0f, 2.0f);

            auto shader = std::make_shared<Circe::Shader>(
                "assets/shaders/ray_march.vert",
                "assets/shaders/ray_march.frag"
            );

            m_Material = std::make_shared<Circe::Material>(shader);
            m_Material->SetColor(glm::vec4(1.0f, 0.4f, 0.2f, 1.0f));
            m_Model = std::make_shared<Circe::Model>(m_QuadMesh, m_Material);

            std::vector<std::string> faces = {
                "assets/skybox/Standard-Cube-Map/px.png",   // +X
                "assets/skybox/Standard-Cube-Map/nx.png",    // -X
                "assets/skybox/Standard-Cube-Map/py.png",     // +Y
                "assets/skybox/Standard-Cube-Map/ny.png",  // -Y
                "assets/skybox/Standard-Cube-Map/pz.png",   // +Z
                "assets/skybox/Standard-Cube-Map/nz.png"     // -Z
            };

            m_SkyTexture = std::make_shared<Circe::Texture>(faces);

        }

        void OnInit() override {
            auto entity = std::make_unique<Circe::Entity>("RayMarchQuad");
            entity->SetModel(m_Model);

            m_BlackHoleEntity.GetTransform().Position = glm::vec3(2.0f, 0.0f, 0.0f);
            m_BlackHoleEntity.AddBehavior<GravityBehavior>(10.0f);
            m_ObjectEntity.GetTransform().Position = glm::vec3(-2.0f, 0.0f, 0.0f);
            m_ObjectEntity.AddBehavior<GravityBehavior>(1.0f);

            AddEntity(std::move(entity));

            UpdateCameraOrbit();
        }

        void OnUpdate(float deltaTime) override {
            const float G = 0.1f; // Gravitational constant

            auto pos1 = m_ObjectEntity.GetTransform().Position;
            auto pos2 = m_BlackHoleEntity.GetTransform().Position;
            auto toBlackHole = pos2 - pos1;
            float distance = glm::length(toBlackHole);
            if (distance < 0.1f) {
                m_ObjectEntity.GetBehavior<GravityBehavior>()->Update(deltaTime);
                m_BlackHoleEntity.GetBehavior<GravityBehavior>()->Update(deltaTime);
                return;
            } // Avoid singularity

            // Simple inverse square law gravity towards the black hole
            float forceMagnitude = G * m_ObjectEntity.GetBehavior<GravityBehavior>()->GetMass() * m_BlackHoleEntity.GetBehavior<GravityBehavior>()->GetMass() / (distance * distance);
            glm::vec3 forceDirection = glm::normalize(toBlackHole);
            glm::vec3 force = forceDirection * forceMagnitude;

            m_ObjectEntity.GetBehavior<GravityBehavior>()->ApplyForce(force);
            m_ObjectEntity.GetBehavior<GravityBehavior>()->Update(deltaTime);
            
            m_BlackHoleEntity.GetBehavior<GravityBehavior>()->ApplyForce(-force); // Apply opposite force to black hole
            m_BlackHoleEntity.GetBehavior<GravityBehavior>()->Update(deltaTime);


        }

        void OnEvent(Circe::Event& event) override {
            // Handle camera orbit controls
            Circe::EventDispatcher dispatcher(event);
            
            // Middle mouse button pressed to orbit
            dispatcher.Dispatch<Circe::MouseButtonPressedEvent>([this](Circe::MouseButtonPressedEvent& e) {
                if (e.GetMouseButton() == 2) {
                    m_IsOrbiting = true;
                    m_HasLastMouse = false;
                    return true;
                }
                return false;
            });

            // Middle mouse button released
            dispatcher.Dispatch<Circe::MouseButtonReleasedEvent>([this](Circe::MouseButtonReleasedEvent& e) {
                if (e.GetMouseButton() == 2) {
                    m_IsOrbiting = false;
                    m_HasLastMouse = false;
                    return true;
                }
                return false;
            });

            // Mouse movement for orbiting if middle mouse button is held
            dispatcher.Dispatch<Circe::MouseMovedEvent>([this](Circe::MouseMovedEvent& e) {
                if (!m_IsOrbiting) {
                    return false;
                }

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
                m_OrbitPitchDegrees -= dy * m_OrbitSensitivity;
                m_OrbitPitchDegrees = glm::clamp(m_OrbitPitchDegrees, -85.0f, 85.0f);

                UpdateCameraOrbit();
                return true;
            });

            // Mouse scroll for zooming
            dispatcher.Dispatch<Circe::MouseScrolledEvent>([this](Circe::MouseScrolledEvent& e) {
                m_OrbitDistance -= e.GetYOffset() * m_ZoomSpeed;
                m_OrbitDistance = glm::clamp(m_OrbitDistance, m_MinOrbitDistance, m_MaxOrbitDistance);
                UpdateCameraOrbit();
                return true;
            });
        }

        void OnRender(Circe::Renderer& renderer) override {
            renderer.SetSkyBoxTexture(m_SkyTexture);

            renderer.SetCustomVec3Uniform("blackHolePos", m_BlackHoleEntity.GetTransform().Position);

            renderer.SetCustomVec3Uniform("objectPos", m_ObjectEntity.GetTransform().Position);
        }    

    private:
        std::shared_ptr<Circe::Mesh> m_QuadMesh;
        std::shared_ptr<Circe::Material> m_Material;
        std::shared_ptr<Circe::Model> m_Model;
        std::shared_ptr<Circe::Texture> m_SkyTexture;

        std::shared_ptr<Circe::Material> m_LightMaterial;
        std::shared_ptr<Circe::Model> m_LightModel;

        Circe::Entity m_BlackHoleEntity;
        Circe::Entity m_ObjectEntity;

       
        Circe::Camera* m_Camera = nullptr;
        glm::vec3 m_CameraTarget = glm::vec3(0.0f);
        glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
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

        void UpdateCameraOrbit() {
            if (!m_Camera) {
                return;
            }

            float yawRad = glm::radians(m_OrbitYawDegrees);
            float pitchRad = glm::radians(m_OrbitPitchDegrees);

            glm::vec3 offset;
            offset.x = m_OrbitDistance * std::cos(pitchRad) * std::cos(yawRad);
            offset.y = m_OrbitDistance * std::sin(pitchRad);
            offset.z = m_OrbitDistance * std::cos(pitchRad) * std::sin(yawRad);

            m_Camera->SetPosition(m_CameraTarget + offset);
            m_Camera->SetLookAt(m_CameraTarget, m_CameraUp);
        }


    };

}

int WIDTH = 1280;
int HEIGHT = 1280;

int main() {
    
    Circe::Engine engine(WIDTH, HEIGHT, "Circe Engine");
    RayMarchingScene scene;

    CIRCE_LOG_INFO("Starting Ray Marching Scene");
    
    // Create a camera and set it on the renderer
    auto camera = std::make_shared<Circe::Camera>(45.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    engine.GetRenderer()->SetCamera(camera);
    scene.SetCamera(camera.get(), glm::vec3(0.0f, 0.0f, 0.0f));
    
    engine.SetScene(&scene);
    engine.Run();
    return 0;
}
