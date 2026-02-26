#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Texture.h"
#include "Resources/SimpleMeshGen.h"
#include "Scene/CameraEntity.h"
#include <Circe.h>
#include <memory>
int WIDTH = 1280;
int HEIGHT = 1280;

namespace {
    class CameraOrbitBehavior : public Circe::Behavior {
    public:
        CameraOrbitBehavior(float distance = 3.0f, float sensitivity = 0.2f)
            : m_OrbitDistance(distance), m_OrbitSensitivity(sensitivity) {}
    

        Circe::CameraEntity* GetOwner() const {
            return Behavior::GetOwnerAs<Circe::CameraEntity>();
        }

        void OnInit() override {
            auto camera = GetOwner();
            if (!camera) {
                throw std::runtime_error("CameraOrbitBehavior must be attached to a Circe::CameraEntity owner.");
            }
            UpdateCameraOrbit();
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
                m_OrbitPitchDegrees += dy * m_OrbitSensitivity;
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
        private:
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
                Circe::Camera* camera = GetOwner()->GetCamera();
                if (!camera) {
                    return;
                }

                float yawRad = glm::radians(m_OrbitYawDegrees);
                float pitchRad = glm::radians(m_OrbitPitchDegrees);

                glm::vec3 offset;
                offset.x = m_OrbitDistance * std::cos(pitchRad) * std::cos(yawRad);
                offset.y = m_OrbitDistance * std::sin(pitchRad);
                offset.z = m_OrbitDistance * std::cos(pitchRad) * std::sin(yawRad);


                camera->SetPosition(camera->GetTarget() + offset);
                camera->SetLookAt(camera->GetTarget(), camera->GetUp());
        }
    };

    class MaterialPreviewScene : public Circe::Scene{
        public:
        MaterialPreviewScene() {
            std::shared_ptr<Circe::Mesh> sphereMesh = Circe::GenerateSphereMesh(1.0f, 64, 64);
            auto shader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/default.frag"
            );

            auto lightShader = std::make_shared<Circe::Shader>(
                "assets/shaders/default.vert",
                "assets/shaders/light.frag"
            );

            
            auto material = std::make_shared<Circe::Material>(shader);
            std::shared_ptr<Circe::Texture> particleTexture = std::make_shared<Circe::Texture>("assets/textures/Ground037_1K-JPG/Ground037_1K-JPG_Color.jpg");
            material->SetTexture("diffuseMap", particleTexture);
            m_Model = std::make_shared<Circe::Model>(sphereMesh, material);

            auto lightMaterial = std::make_shared<Circe::Material>(lightShader);
            m_MainLightModel = std::make_shared<Circe::Model>(sphereMesh, lightMaterial);


        }

        void OnInit() override {
            std::vector<std::string> faces = {
                "assets/skybox/Standard-Cube-Map/px.png",   // +X
                "assets/skybox/Standard-Cube-Map/nx.png",    // -X
                "assets/skybox/Standard-Cube-Map/py.png",     // +Y
                "assets/skybox/Standard-Cube-Map/ny.png",  // -Y
                "assets/skybox/Standard-Cube-Map/pz.png",   // +Z
                "assets/skybox/Standard-Cube-Map/nz.png"     // -Z
            };

            m_SkyTexture = std::make_shared<Circe::Texture>(faces);

            auto sphereEntity = std::make_unique<Circe::Entity>("Sphere");
            sphereEntity->SetModel(m_Model);
            AddEntity(std::move(sphereEntity));

            
            auto cameraEntity = std::make_unique<Circe::CameraEntity>("Camera", m_Camera);
            cameraEntity->AddBehavior<CameraOrbitBehavior>();
            
            AddEntity(std::move(cameraEntity));
            
            auto mainLightEntity = std::make_unique<Circe::Entity>("MainLight");
            mainLightEntity->SetModel(m_MainLightModel);
            m_MainLight = mainLightEntity->AddBehavior<Circe::Light>(Circe::LightType::Point);
            mainLightEntity->GetTransform().Position = glm::vec3(0.0f, 10.0f, 10.0f);
            AddEntity(std::move(mainLightEntity));
        }


        void OnRender(Circe::Renderer& renderer) override {
            renderer.SetSkyBoxTexture(m_SkyTexture);
            if (m_MainLight) {
                renderer.SetLight(*m_MainLight);
            }
        }


        void SetCamera(std::shared_ptr<Circe::Camera> camera) {
            m_Camera = camera;

        }

            

        private:
            std::shared_ptr<Circe::Model> m_Model;
            std::shared_ptr<Circe::Model> m_MainLightModel;
            std::shared_ptr<Circe::Texture> m_SkyTexture;
            std::shared_ptr<Circe::Camera> m_Camera;
            Circe::Light* m_MainLight;

    };
}


int main() {
    
    Circe::Engine engine(WIDTH, HEIGHT, "Circe Engine");
    MaterialPreviewScene scene;

    CIRCE_LOG_INFO("Starting Material Preview Scene");
    
    // Create a camera and set it on the renderer
    auto camera = std::make_shared<Circe::Camera>(45.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    engine.GetRenderer()->SetCamera(camera);
    scene.SetCamera(camera);

    
    engine.SetScene(&scene);
    engine.Run();
    return 0;
}
