#include "glm/fwd.hpp"
#include "pch.h"

#include "Renderer.h"
#include "../Core/Log.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Circe {

    Renderer::Renderer()
        : m_ClearColor(0.1f, 0.1f, 0.1f, 1.0f) {
    }

    Renderer::~Renderer() {
    }

    void Renderer::Initialize() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::Critical("Failed to initialize GLAD");
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        Log::Info("OpenGL Renderer: " + std::string(reinterpret_cast<const char*>(renderer)));
        Log::Info("OpenGL Version: " + std::string(reinterpret_cast<const char*>(version)));


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

    void Renderer::SetLight(const Light& light) {
        m_LightPosition = light.GetWorldPosition();
        m_LightColor = light.GetColor();
        m_LightIntensity = light.GetIntensity();
    }

    void Renderer::SetSkyBoxTexture(std::shared_ptr<Texture> cubemap) {
        m_SkyboxTexture = cubemap;
    }

    void Renderer::SetCustomVec3Uniform(const std::string& name, glm::vec3 value) {
        //Temporary method for setting custom uniforms, will be replaced with proper material system later
        m_CustomUniforms[name] = value;
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

            if (m_SkyboxTexture) {
                m_SkyboxTexture->BindCubemap(0);
                cmd.material->GetShader()->SetInt("skybox", 0);
            }

            auto shader = cmd.material->GetShader();
            shader->SetMat4("projection", m_Camera->GetProjectionMatrix());
            shader->SetMat4("view", m_Camera->GetViewMatrix());
            shader->SetMat4("model", cmd.modelMatrix);
            shader->SetVec3("lightPos", m_LightPosition);
            shader->SetVec3("lightColor", m_LightColor);
            shader->SetVec3("cameraPos", m_Camera->GetPosition());
            shader->SetFloat("lightIntensity", m_LightIntensity);
            shader->SetFloat("ambientStrength", m_AmbientStrength);

            for (const auto& uniform : m_CustomUniforms) {
                shader->SetVec3(uniform.first.c_str(), uniform.second);
            }

            cmd.mesh->Bind();
            glDrawElements(GL_TRIANGLES, cmd.mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
            cmd.mesh->Unbind();
        }

        m_RenderQueue.clear();
    }

}