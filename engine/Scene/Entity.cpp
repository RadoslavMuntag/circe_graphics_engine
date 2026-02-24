#include "pch.h"

#include "Entity.h"
#include "../Renderer/Model.h"
#include "../Renderer/Renderer.h"

namespace Circe {
    
    void Entity::OnUpdate(float deltaTime) {
        for (auto& behavior : m_Behaviors) {
            if (behavior) {
                behavior->OnUpdate(deltaTime);
            }
        }
    }
    
    void Entity::OnRender(Renderer& renderer) {
        if (m_Model) {
            m_Model->Render(renderer, m_Transform.GetModelMatrix());
        }
    }

    void Entity::OnEvent(Event& event) {
        for (auto& behavior : m_Behaviors) {
            if (behavior) {
                behavior->OnEvent(event);
                if (event.Handled) {
                    break;
                }
            }
        }
    }

}
