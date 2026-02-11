#pragma once

#include <memory>
#include "Math/Transform.h"

namespace Circe {

    class Mesh;
    class Material;
    class Renderer;

    class Model {
    public:
        Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        ~Model();

        void Render(Renderer& renderer, const glm::mat4& parentMatrix = glm::mat4(1.0f));

        void SetPosition(const glm::vec3& pos) { m_Transform.Position = pos; }
        void SetScale(const glm::vec3& scale) { m_Transform.Scale = scale; }
        void SetRotation(const glm::vec3& eulerRadians) { m_Transform.Rotation = glm::quat(eulerRadians); }

        glm::vec3 GetPosition() const { return m_Transform.Position; }
        glm::vec3 GetScale() const { return m_Transform.Scale; }
        glm::vec3 GetRotation() const { return glm::eulerAngles(m_Transform.Rotation); }

        glm::mat4 GetModelMatrix() const;

        Transform& GetTransform() { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
        std::shared_ptr<Material> GetMaterial() const { return m_Material; }

    private:
        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
        Transform m_Transform;
    };

}
