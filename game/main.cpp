#include <Circe.h>

#include "Behaviors/GravityBehavior.h"
#include "Behaviors/CameraOrbitBehavior.h"
#include "Scene/CameraEntity.h"

#include <glm/glm.hpp>
#include <memory>

namespace {

    class RayMarchingScene : public Circe::Scene {
    public:

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
            AddEntity(std::move(entity));

            auto blackHoleEntity = std::make_unique<Circe::Entity>("BlackHole");
            blackHoleEntity->GetTransform().Position = glm::vec3(2.0f, 0.0f, 0.0f);
            blackHoleEntity->AddBehavior<Game::GravityBehavior>(10.0f);
            AddEntity(std::move(blackHoleEntity));

            auto objectEntity = std::make_unique<Circe::Entity>("Object");
            objectEntity->GetTransform().Position = glm::vec3(-2.0f, 0.0f, 0.0f);
            objectEntity->AddBehavior<Game::GravityBehavior>(1.0f);
            AddEntity(std::move(objectEntity));

            auto cameraEntity = std::make_unique<Circe::CameraEntity>("Camera", m_Camera);
            cameraEntity->AddBehavior<Game::CameraOrbitBehavior>(5.0f, 0.3f);
            AddEntity(std::move(cameraEntity));

        }

        void OnUpdate(float deltaTime) override {
            const float G = 0.1f; // Gravitational constant

            auto pos1 = GetEntity("BlackHole")->GetTransform().Position;
            auto pos2 = GetEntity("Object")->GetTransform().Position;
            auto toBlackHole = pos2 - pos1;
            float distance = glm::length(toBlackHole);
            if (distance < 0.1f) 
                return; // Avoid singularity

            // Simple inverse square law gravity towards the black hole
            float forceMagnitude = G * GetEntity("Object")->GetBehavior<Game::GravityBehavior>()->GetMass() * GetEntity("BlackHole")->GetBehavior<Game::GravityBehavior>()->GetMass() / (distance * distance);
            glm::vec3 forceDirection = glm::normalize(toBlackHole);
            glm::vec3 force = forceDirection * forceMagnitude;

            GetEntity("Object")->GetBehavior<Game::GravityBehavior>()->ApplyForce(force);
            
            GetEntity("BlackHole")->GetBehavior<Game::GravityBehavior>()->ApplyForce(-force); // Apply opposite force to black hole


        }

        void OnRender(Circe::Renderer& renderer) override {
            renderer.SetSkyBoxTexture(m_SkyTexture);

            renderer.SetCustomVec3Uniform("blackHolePos", GetEntity("BlackHole")->GetTransform().Position);

            renderer.SetCustomVec3Uniform("objectPos", GetEntity("Object")->GetTransform().Position);
        }
        
        void SetCamera(std::shared_ptr<Circe::Camera> camera, const glm::vec3& target) {
            m_Camera = camera;
            m_Camera->SetLookAt(target);
        }

    private:
        std::shared_ptr<Circe::Mesh> m_QuadMesh;
        std::shared_ptr<Circe::Material> m_Material;
        std::shared_ptr<Circe::Model> m_Model;
        std::shared_ptr<Circe::Texture> m_SkyTexture;

        std::shared_ptr<Circe::Material> m_LightMaterial;
        std::shared_ptr<Circe::Model> m_LightModel;

        std::shared_ptr<Circe::Camera> m_Camera = nullptr;
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
    scene.SetCamera(camera, glm::vec3(0.0f, 0.0f, 0.0f));
    
    engine.SetScene(&scene);
    engine.Run();
    return 0;
}
