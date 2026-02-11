#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Circe {

    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void Use() const;
        void SetInt(const char* name, int value) const;
        void SetFloat(const char* name, float value) const;
        void SetVec4(const char* name, const glm::vec4& value) const;
        void SetMat4(const char* name, const glm::mat4& value) const;

    private:
        unsigned int m_ID = 0;
    };

}
