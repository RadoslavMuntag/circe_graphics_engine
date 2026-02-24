#include "pch.h"

#include "SimpleMeshGen.h"
#include "../Renderer/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>

namespace Circe {
    std::shared_ptr<Circe::Mesh> GenerateQuadMesh(float width, float height) {
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        std::vector<Circe::Vertex> vertices = {
            { { -halfWidth, -halfHeight, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { {  halfWidth, -halfHeight, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { halfWidth,  halfHeight, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { -halfWidth,  halfHeight, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };

        return std::make_shared<Circe::Mesh>(vertices, indices);
    }

    // function to generate a 3d spherical particle mesh
    std::shared_ptr<Circe::Mesh> GenerateSphereMesh(float radius, unsigned int sectorCount, unsigned int stackCount) {
        std::vector<Circe::Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= stackCount; ++i) {
            float stackAngle = glm::pi<float>() / 2.0f - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle); // r * cos(u)
            float z = radius * sinf(stackAngle);  // r * sin(u)

            for (int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * 2.0f * glm::pi<float>() / sectorCount; // from 0 to 2pi

                float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                vertices.push_back({ { x, y, z }, glm::normalize(glm::vec3(x, y, z)), { (float)j / sectorCount, (float)i / stackCount } });
            }
        }

        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1);     // beginning of current stack
            int k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

    

        return std::make_shared<Circe::Mesh>(vertices, indices);
    }

    // genereate a cube mesh for testing
    std::shared_ptr<Circe::Mesh>GenerateCubeMesh(float size) {
        float halfSize = size / 2.0f;
        std::vector<Circe::Vertex> vertices = {
            // Front face
            { { -halfSize, -halfSize,  halfSize }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { {  halfSize, -halfSize,  halfSize }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { halfSize,  halfSize,  halfSize }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { -halfSize,  halfSize,  halfSize }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            // Back face
            { { -halfSize, -halfSize, -halfSize }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
            { { -halfSize,  halfSize, -halfSize }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
            { { halfSize,  halfSize, -halfSize }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
            { { halfSize, -halfSize, -halfSize }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
            // Left face
            { { -halfSize, -halfSize, -halfSize }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
            { { -halfSize, -halfSize,  halfSize }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -halfSize,  halfSize,  halfSize }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -halfSize,  halfSize, -halfSize }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
            // Right face
            { { halfSize, -halfSize, -halfSize }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { halfSize,  halfSize, -halfSize }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { halfSize,  halfSize,  halfSize }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
            { { halfSize, -halfSize,  halfSize }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
            // Top face
            { { -halfSize,  halfSize, -halfSize }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { -halfSize,  halfSize,  halfSize }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { halfSize,  halfSize,  halfSize }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { halfSize,  halfSize, -halfSize }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
            // Bottom face
            { { -halfSize, -halfSize, -halfSize }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { halfSize, -halfSize, -halfSize }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { halfSize, -halfSize,  halfSize }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { -halfSize, -halfSize,  halfSize }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 0, 2, 3,       // Front face
            4, 5, 6, 4, 6, 7,       // Back face
            8, 9, 10, 8, 10, 11,    // Left face
            12, 13, 14, 12, 14, 15, // Right face
            16, 17, 18, 16, 18, 19, // Top face
            20, 21, 22, 20, 22, 23  // Bottom face
        };

        return std::make_shared<Circe::Mesh>(vertices, indices);
    }
}