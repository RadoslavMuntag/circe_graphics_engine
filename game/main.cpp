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
    class ParticleSystemScene : public Circe::Scene {
    public:
        ParticleSystemScene() {
            m_ParticleMesh = Circe::GenerateSphereMesh(0.1f, 16, 16);

            auto shader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/default.frag"
            );
            m_ParticleMaterial = std::make_shared<Circe::Material>(shader);
            m_ParticleMaterial->SetColor(glm::vec4(1.0f, 0.4f, 0.2f, 1.0f));
        }

        void OnInit() override {
            // Create a simple particle entity
            auto entity = std::make_unique<Circe::Entity>("Particle");
            m_ParticleModel = std::make_shared<Circe::Model>(m_ParticleMesh, m_ParticleMaterial);
            entity->SetModel(m_ParticleModel);
            AddEntity(std::move(entity));
        }

        void OnUpdate(float deltaTime) override {
            // Update particle position or properties here
            auto entity = GetEntity("Particle");
            if (entity) {
                auto& transform = entity->GetTransform();
                //transform.Position += glm::vec3(0.0f, deltaTime, 0.0f); // Move up over time
            }
        }

    private:
        std::shared_ptr<Circe::Mesh> m_ParticleMesh;
        std::shared_ptr<Circe::Material> m_ParticleMaterial;
        std::shared_ptr<Circe::Model> m_ParticleModel;


    };

}

int WIDTH = 720;
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
