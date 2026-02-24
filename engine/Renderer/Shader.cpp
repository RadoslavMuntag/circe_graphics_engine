#include "pch.h"

#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Circe {

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        // Read vertex shader source
        std::ifstream vFile(vertexPath);
        if (!vFile.is_open()) {
            throw std::runtime_error("Failed to open vertex shader: " + vertexPath);
        }
        std::stringstream vBuffer;
        vBuffer << vFile.rdbuf();
        std::string vCode = vBuffer.str();
        const char* vCodeCStr = vCode.c_str();

        // Read fragment shader source
        std::ifstream fFile(fragmentPath);
        if (!fFile.is_open()) {
            throw std::runtime_error("Failed to open fragment shader: " + fragmentPath);
        }
        std::stringstream fBuffer;
        fBuffer << fFile.rdbuf();
        std::string fCode = fBuffer.str();
        const char* fCodeCStr = fCode.c_str();

        // Compile vertex shader
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCodeCStr, nullptr);
        glCompileShader(vertex);

        // Check for vertex compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            throw std::runtime_error("Vertex shader compilation failed: " + std::string(infoLog));
        }

        // Compile fragment shader
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCodeCStr, nullptr);
        glCompileShader(fragment);

        // Check for fragment compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            throw std::runtime_error("Fragment shader compilation failed: " + std::string(infoLog));
        }

        // Link shaders
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);

        // Check for linking errors
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
            throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader() {
        if (m_ID) {
            glDeleteProgram(m_ID);
        }
    }

    void Shader::Use() const {
        glUseProgram(m_ID);
    }
    
    void Shader::SetBool(const char* name, bool value) const {
        glUniform1i(glGetUniformLocation(m_ID, name), (int)value);
    }

    void Shader::SetInt(const char* name, int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name), value);
    }

    void Shader::SetFloat(const char* name, float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name), value);
    }

    void Shader::SetVec3(const char* name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z);
    }

    void Shader::SetVec4(const char* name, const glm::vec4& value) const {
        glUniform4f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat4(const char* name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, glm::value_ptr(value));
    }

}
