#include "Scene.h"
#include "../Renderer/Renderer.h"

namespace Circe {

    void Scene::Update(float deltaTime) {
        OnUpdate(deltaTime);

        for (auto& entity : m_Entities) {
            if (entity && entity->IsActive()) {
                entity->OnUpdate(deltaTime);
            }
        }
    }

    void Scene::Render(Renderer& renderer) {
        OnRender(renderer);

        for (auto& entity : m_Entities) {
            if (entity && entity->IsActive()) {
                entity->OnRender(renderer);
            }
        }

        renderer.Flush();
    }

    void Scene::AddEntity(std::unique_ptr<Entity> entity) {
        if (entity) {
            m_Entities.push_back(std::move(entity));
        }
    }

    Entity* Scene::GetEntity(const std::string& name) {
        for (auto& entity : m_Entities) {
            if (entity && entity->GetName() == name) {
                return entity.get();
            }
        }
        return nullptr;
    }

}
