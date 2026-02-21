#include "Core/Log.h"
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

    class ParticleSystemScene : public Circe::Scene {
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

        ParticleSystemScene() {
            m_ParticleMesh = Circe::GenerateSphereMesh(0.02f, 16, 16);

            auto shader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/default.frag"
            );
            auto lightShader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/light.frag"
            );
            m_ParticleMaterial = std::make_shared<Circe::Material>(shader);
            m_ParticleMaterial->SetColor(glm::vec4(1.0f, 0.4f, 0.2f, 1.0f));

            std::shared_ptr<Circe::Texture> particleTexture = std::make_shared<Circe::Texture>("assets/textures/Ground037_1K-JPG/Ground037_1K-JPG_Color.jpg");
            m_ParticleMaterial->SetTexture("diffuseMap", particleTexture);
            m_ParticleModel = std::make_shared<Circe::Model>(m_ParticleMesh, m_ParticleMaterial);

            m_LightMaterial = std::make_shared<Circe::Material>(lightShader);
            m_LightModel = std::make_shared<Circe::Model>(m_ParticleMesh, m_LightMaterial);
            m_LightModel->SetScale(glm::vec3(2.5f));
        }

        void OnInit() override {
            // Create a light entity
            auto lightEntity = std::make_unique<Circe::Entity>("MainLight");
            lightEntity->GetTransform().Position = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 lightPosition = lightEntity->GetTransform().Position;
            m_LightEntity = lightEntity.get();
            m_MainLight = lightEntity->AddBehavior<Circe::Light>(Circe::LightType::Point);
            m_MainLight->SetColor(glm::vec3(1.0f, 0.8f, 0.4f));
            m_MainLight->SetIntensity(1.2f);
            lightEntity->SetModel(m_LightModel); // Optional: visualize the light source
            lightEntity->AddBehavior<GravityBehavior>(0.8f);
            AddEntity(std::move(lightEntity));

            // Create a simple particle entity
            for (int i = 0; i < 2; ++i) {
                auto entity = std::make_unique<Circe::Entity>("Particle_" + std::to_string(i));

                entity->SetModel(m_ParticleModel);
                // Add gravity behavior
                auto gravBehavior = entity->AddBehavior<GravityBehavior>(0.02f);
                
                // Set random position
                entity->GetTransform().Position = glm::vec3(
                    (rand() % 100) / 50.0f - 1.0f,
                    (rand() % 100) / 50.0f - 1.0f,
                    0.0f
                );

                glm::vec3 toLight = lightPosition - entity->GetTransform().Position;
                glm::vec3 direction = glm::normalize(toLight);
                glm::vec3 up(0.0f, 1.0f, 0.0f);
                if (glm::abs(glm::dot(direction, up)) > 0.98f) {
                    up = glm::vec3(1.0f, 0.0f, 0.0f);
                }

                glm::vec3 perpendicular = glm::normalize(glm::cross(direction, up));
                gravBehavior->SetVelocity(perpendicular * 0.2f);


                AddEntity(std::move(entity));
            }

            UpdateCameraOrbit();
        }

        void OnUpdate(float deltaTime) override {
            (void)deltaTime;

            if (m_Camera && m_LightEntity) {
                m_CameraTarget = m_LightEntity->GetTransform().Position;
                UpdateCameraOrbit();
            }

            // Simple gravity between particles
            const float G = 0.1f; // Gravitational constant

            for (auto& entity1 : m_Entities) {
                auto gravBehavior = entity1->GetBehavior<GravityBehavior>();
                if (!gravBehavior) continue;

                for (auto& entity2 : m_Entities) {
                    if (entity1.get() == entity2.get()) continue;
                    auto gravBehavior2 = entity2->GetBehavior<GravityBehavior>();
                    if (!gravBehavior2) continue;


                    auto pos1 = entity1->GetTransform().Position;
                    auto pos2 = entity2->GetTransform().Position;
                    
                    glm::vec3 direction = pos2 - pos1;
                    float distance = glm::length(direction);
                    
                    if (distance < 0.1f) continue; // Avoid division by zero
                    
                    // F = G * m1 * m2 / r^2
                    float force = G * gravBehavior->GetMass() * gravBehavior2->GetMass() / (distance * distance);
                    glm::vec3 forceVector = glm::normalize(direction) * force;
                    
                    gravBehavior->ApplyForce(forceVector);
                }
            }
        }

        void OnEvent(Circe::Event& event) override {
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

            dispatcher.Dispatch<Circe::MouseScrolledEvent>([this](Circe::MouseScrolledEvent& e) {
                m_OrbitDistance -= e.GetYOffset() * m_ZoomSpeed;
                m_OrbitDistance = glm::clamp(m_OrbitDistance, m_MinOrbitDistance, m_MaxOrbitDistance);
                UpdateCameraOrbit();
                return true;
            });
        }

        void OnRender(Circe::Renderer& renderer) override {
            if (m_MainLight) {
                renderer.SetLight(*m_MainLight);
            }
        }

    private:
        std::shared_ptr<Circe::Mesh> m_ParticleMesh;
        std::shared_ptr<Circe::Material> m_ParticleMaterial;
        std::shared_ptr<Circe::Model> m_ParticleModel;

        std::shared_ptr<Circe::Material> m_LightMaterial;
        std::shared_ptr<Circe::Model> m_LightModel;

        Circe::Entity* m_LightEntity = nullptr;
        Circe::Light* m_MainLight = nullptr;
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
    ParticleSystemScene scene;

    CIRCE_LOG_INFO("Starting Particle System Scene");
    
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
