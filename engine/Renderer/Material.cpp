#include "pch.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"

namespace Circe {

    Material::Material(std::shared_ptr<Shader> shader)
        : m_Shader(shader) {
    }

    Material::~Material() {
    }

    void Material::Bind() const {
        if (m_Shader) {
            m_Shader->Use();
            m_Shader->SetVec4("color", m_Color);
        }

        // Bind textures
        int textureUnit = 0;
        for (const auto& [name, texture] : m_Textures) {
            if (texture) {
                texture->Bind(textureUnit);
                if (m_Shader) {
                    m_Shader->SetBool("hasTexture", true);
                    m_Shader->SetInt(name.c_str(), textureUnit);
                }
                textureUnit++;
            }
        }
    }

    void Material::SetTexture(const std::string& name, std::shared_ptr<Texture> texture) {
        m_Textures[name] = texture;
    }

}
