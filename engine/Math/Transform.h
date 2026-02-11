#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Circe {

    class Transform {
    public:
        Transform()
            : Position(0.0f), Rotation(1.0f, 0.0f, 0.0f, 0.0f), Scale(1.0f) {}

        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;

        glm::mat4 GetModelMatrix() const {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
            glm::mat4 rotation = glm::mat4_cast(Rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
            return translation * rotation * scale;
        }

        glm::vec3 Forward() const {
            return Rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        }

        glm::vec3 Right() const {
            return Rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        }

        glm::vec3 Up() const {
            return Rotation * glm::vec3(0.0f, 1.0f, 0.0f);
        }
    };

}


