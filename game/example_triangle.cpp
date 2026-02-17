#include <Core/Engine.h>
#include <Renderer/Camera.h>
#include <Renderer/Material.h>
#include <Renderer/Mesh.h>
#include <Renderer/Model.h>
#include <Renderer/Renderer.h>
#include <Renderer/Shader.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>

namespace {

    class TriangleScene : public Circe::Scene {
    public:
        TriangleScene() {
            std::vector<Circe::Vertex> vertices = {
                { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
                { {  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } }
            };
            std::vector<unsigned int> indices = { 0, 1, 2 };

            m_Mesh = std::make_shared<Circe::Mesh>(vertices, indices);

            auto shader = std::make_shared<Circe::Shader>(
                "../../assets/shaders/triangle.vert",
                "../../assets/shaders/triangle.frag"
            );
            m_Material = std::make_shared<Circe::Material>(shader);
            m_Material->SetColor(glm::vec4(1.0f, 0.4f, 0.2f, 1.0f));

            m_Model = std::make_shared<Circe::Model>(m_Mesh, m_Material);
        }

        void OnInit() override {
            // Create a triangle entity with the model
            auto entity = std::make_unique<Circe::Entity>("Triangle");
            entity->SetModel(m_Model);
            AddEntity(std::move(entity));
        }

        void OnUpdate(float deltaTime) override {
            // Optional: Update the triangle's transform here
            auto entity = GetEntity("Triangle");
            if (entity) {
                entity->GetTransform().Rotation = glm::quat(glm::radians(glm::vec3(deltaTime , deltaTime, 0.0f))) * entity->GetTransform().Rotation;
            }
        }

    private:
        std::shared_ptr<Circe::Mesh> m_Mesh;
        std::shared_ptr<Circe::Material> m_Material;
        std::shared_ptr<Circe::Model> m_Model;
    };

}

int main() {
    Circe::Engine engine(1280, 720, "Circe Engine");
    TriangleScene scene;
    
    // Create a camera and set it on the renderer
    auto camera = std::make_shared<Circe::Camera>(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    engine.GetRenderer()->SetCamera(camera);
    
    engine.SetScene(&scene);
    engine.Run();
    return 0;
}
