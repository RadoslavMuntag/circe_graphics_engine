#pragma once

#include <memory>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace Circe {

    class Shader;
    class Texture;

    class Material {
    public:
        Material(std::shared_ptr<Shader> shader);
        ~Material();

        void Bind() const;
        void SetTexture(const std::string& name, std::shared_ptr<Texture> texture);
        void SetColor(const glm::vec4& color) { m_Color = color; }
        glm::vec4 GetColor() const { return m_Color; }
        std::shared_ptr<Shader> GetShader() const { return m_Shader; }

    private:
        std::shared_ptr<Shader> m_Shader;
        std::map<std::string, std::shared_ptr<Texture>> m_Textures;
        glm::vec4 m_Color = glm::vec4(1.0f);
    };

}
