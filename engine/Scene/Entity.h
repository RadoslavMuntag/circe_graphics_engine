#pragma once

#include "../Math/Transform.h"
#include <string>
#include <memory>

namespace Circe {

    class Renderer;
    class Model;

    class Entity {
    public:
        Entity(const std::string& name = "Entity")
            : m_Name(name), m_Active(true) {}

        virtual ~Entity() = default;

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender(Renderer& renderer);

        Transform& GetTransform() { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        bool IsActive() const { return m_Active; }
        void SetActive(bool active) { m_Active = active; }

        void SetModel(std::shared_ptr<Model> model) { m_Model = model; }
        std::shared_ptr<Model> GetModel() const { return m_Model; }

    protected:
        Transform m_Transform;
        std::string m_Name;
        bool m_Active;
        std::shared_ptr<Model> m_Model;
    };

}
