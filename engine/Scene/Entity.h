#pragma once

#include "../Core/Export.h"
#include "../Math/Transform.h"
#include "../Events/Event.h"
#include <string>
#include <memory>
#include <memory>
#include <vector>
#include "Behavior.h"

namespace Circe {

    class Renderer;
    class Model;

    class CIRCE_API Entity {
    public:
        Entity(const std::string& name = "Entity")
            : m_Name(name), m_Active(true) {}

        virtual ~Entity() = default;

        virtual void OnUpdate(float deltaTime);
        virtual void OnRender(Renderer& renderer);
        virtual void OnEvent(Event& event);

        Transform& GetTransform() { return m_Transform; }
        const Transform& GetTransform() const { return m_Transform; }

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        bool IsActive() const { return m_Active; }
        void SetActive(bool active) { m_Active = active; }

        void SetModel(std::shared_ptr<Model> model) { m_Model = model; }
        std::shared_ptr<Model> GetModel() const { return m_Model; }

        template<typename T, typename... Args>
        T* AddBehavior(Args&&... args) {
            auto behavior = std::make_unique<T>(std::forward<Args>(args)...);
            behavior->SetOwner(this);
            T* ptr = behavior.get();
            behavior->OnInit();
            m_Behaviors.push_back(std::move(behavior));
            return ptr;
        }

        template<typename T>
        T* GetBehavior() {
            for (auto& b : m_Behaviors) {
                if (auto* ptr = dynamic_cast<T*>(b.get())) {
                    return ptr;
                }
            }
            return nullptr;
        }


    protected:
        Transform m_Transform;
        std::string m_Name;
        bool m_Active;
        std::shared_ptr<Model> m_Model;
        std::vector<std::unique_ptr<Behavior>> m_Behaviors;
    };

}
