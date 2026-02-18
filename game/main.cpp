#include <Core/Engine.h>
#include <Renderer/Camera.h>
#include <Renderer/Material.h>
#include <Renderer/Mesh.h>
#include <Renderer/Model.h>
#include <Renderer/Renderer.h>
#include <Renderer/Shader.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>
#include <Resources/SimpleMeshGen.h>

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

        float GetMass() const { return m_Mass; }
        glm::vec3 GetVelocity() const { return m_Velocity; }

    private:
        float m_Mass;
        glm::vec3 m_Velocity;
        glm::vec3 m_Acceleration;
    };

    class ParticleSystemScene : public Circe::Scene {
    public:
        ParticleSystemScene() {
            m_ParticleMesh = Circe::GenerateSphereMesh(0.02f, 16, 16);

            auto shader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/default.frag"
            );
            m_ParticleMaterial = std::make_shared<Circe::Material>(shader);
            m_ParticleMaterial->SetColor(glm::vec4(1.0f, 0.4f, 0.2f, 1.0f));
            m_ParticleModel = std::make_shared<Circe::Model>(m_ParticleMesh, m_ParticleMaterial);
        }

        void OnInit() override {
            // Create a simple particle entity
            for (int i = 0; i < 19; ++i) {
                auto entity = std::make_unique<Circe::Entity>("Particle_" + std::to_string(i));

                entity->SetModel(m_ParticleModel);
                // Add gravity behavior
                auto gravBehavior = entity->AddBehavior<GravityBehavior>(0.2f);
                
                // Set random position
                entity->GetTransform().Position = glm::vec3(
                    (rand() % 100) / 50.0f - 1.0f,
                    (rand() % 100) / 50.0f - 1.0f,
                    0.0f
                );


                AddEntity(std::move(entity));
            }
        }

         void OnUpdate(float deltaTime) override {
            // Simple gravity between particles
            const float G = 0.1f; // Gravitational constant

            for (auto& entity1 : m_Entities) {
                auto gravBehavior = entity1->GetBehavior<GravityBehavior>();
                if (!gravBehavior) continue;

                for (auto& entity2 : m_Entities) {
                    if (entity1.get() == entity2.get()) continue;

                    auto pos1 = entity1->GetTransform().Position;
                    auto pos2 = entity2->GetTransform().Position;
                    
                    glm::vec3 direction = pos2 - pos1;
                    float distance = glm::length(direction);
                    
                    if (distance < 0.1f) continue; // Avoid division by zero
                    
                    // F = G * m1 * m2 / r^2
                    float force = G * gravBehavior->GetMass() / (distance * distance);
                    glm::vec3 forceVector = glm::normalize(direction) * force;
                    
                    gravBehavior->ApplyForce(forceVector);
                }
            }
        }

    private:
        std::shared_ptr<Circe::Mesh> m_ParticleMesh;
        std::shared_ptr<Circe::Material> m_ParticleMaterial;
        std::shared_ptr<Circe::Model> m_ParticleModel;


    };

}

int WIDTH = 1280;
int HEIGHT = 1280;

int main() {
    
    Circe::Engine engine(WIDTH, HEIGHT, "Circe Engine");
    ParticleSystemScene scene;
    
    // Create a camera and set it on the renderer
    auto camera = std::make_shared<Circe::Camera>(45.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    engine.GetRenderer()->SetCamera(camera);
    
    engine.SetScene(&scene);
    engine.Run();
    return 0;
}
