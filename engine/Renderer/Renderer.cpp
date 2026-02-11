#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Circe {

    Renderer::Renderer()
        : m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f) {
    }

    Renderer::~Renderer() {
    }

    void Renderer::Initialize() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_Initialized = true;
    }

    void Renderer::Clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Present() {
        // Swap is handled by Window, this is for future post-processing
    }

    void Renderer::SetViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4& color) {
        m_ClearColor = color;
    }

    void Renderer::DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
        // TODO: Implement with VAO/VBO
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size) {
        // TODO: Implement with VAO/VBO
    }

    void Renderer::SubmitMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::mat4& modelMatrix) {
        if (mesh && material) {
            m_RenderQueue.push_back({ mesh, material, modelMatrix });
        }
    }

    void Renderer::Flush() {
        if (!m_Camera) {
            return;
        }

        for (const auto& cmd : m_RenderQueue) {
            cmd.material->Bind();
            
            // Set matrix uniforms
            cmd.material->GetShader()->SetMat4("projection", m_Camera->GetProjectionMatrix());
            cmd.material->GetShader()->SetMat4("view", m_Camera->GetViewMatrix());
            cmd.material->GetShader()->SetMat4("model", cmd.modelMatrix);

            cmd.mesh->Bind();
            glDrawElements(GL_TRIANGLES, cmd.mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
            cmd.mesh->Unbind();
        }

        m_RenderQueue.clear();
    }

}