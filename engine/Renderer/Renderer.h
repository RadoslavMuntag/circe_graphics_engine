#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Circe {

    class Camera;
    class Mesh;
    class Material;

    struct RenderCommand {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        glm::mat4 modelMatrix;
    };

    class Renderer {
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
    };

}
