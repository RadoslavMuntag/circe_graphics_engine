#pragma once

#include <glm/glm.hpp>

namespace Circe {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

        void Bind() const;
        void Unbind() const;
        unsigned int GetIndexCount() const { return m_IndexCount; }

    private:
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;
        unsigned int m_IndexCount = 0;
    };

}
