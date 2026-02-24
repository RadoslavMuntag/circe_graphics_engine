#pragma once

#include "../Core/Export.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Circe {

    class Camera;
    class Mesh;
    class Material;
    class Light;

    struct RenderCommand {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        glm::mat4 modelMatrix;
    };

    class CIRCE_API Renderer {
    public:
        Renderer();
        ~Renderer();

        void Initialize();
        void Clear(const glm::vec4& color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        void Present();

        void SetViewport(int x, int y, int width, int height);
        void SetClearColor(const glm::vec4& color);
        void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; }
        std::shared_ptr<Camera> GetCamera() const { return m_Camera; }

        void SetLight(const Light& light);
        void SetLightPosition(const glm::vec3& position) { m_LightPosition = position; }
        void SetLightColor(const glm::vec3& color) { m_LightColor = color; }
        void SetLightIntensity(float intensity) { m_LightIntensity = intensity; }
        void SetAmbientStrength(float strength) { m_AmbientStrength = strength; }
        void SetSkyBoxTexture(std::shared_ptr<Texture> cubemap);

        void SetCustomVec3Uniform(const std::string& name, glm::vec3 value);

        // Render submission
        void SubmitMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::mat4& modelMatrix);
        void Flush();

        // Drawing primitives
        void DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
        void DrawQuad(const glm::vec3& position, const glm::vec2& size);

    private:
        glm::vec4 m_ClearColor;
        bool m_Initialized = false;
        std::shared_ptr<Camera> m_Camera;
        std::vector<RenderCommand> m_RenderQueue;
        glm::vec3 m_LightPosition = glm::vec3(0.0f, 2.0f, 0.0f);
        glm::vec3 m_LightColor = glm::vec3(1.0f);
        std::shared_ptr<Texture> m_SkyboxTexture;
        float m_LightIntensity = 1.0f;
        float m_AmbientStrength = 0.1f;

        std::unordered_map<std::string, glm::vec3> m_CustomUniforms;
    };

}
