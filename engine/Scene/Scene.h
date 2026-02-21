#pragma once

#include "../Core/Export.h"
#include "../Events/Event.h"
#include "Entity.h"
#include <vector>
#include <memory>

namespace Circe {

    class Renderer;

    class CIRCE_API Scene {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnInit() {}
        virtual void OnShutdown() {}
        virtual void OnUpdate(float deltaTime) { (void)deltaTime; }
        virtual void OnRender(Renderer& renderer) { (void)renderer; }
        virtual void OnEvent(Event& event) { (void)event; }

        void Update(float deltaTime);
        void Render(Renderer& renderer);
        void DispatchEvent(Event& event);

        void AddEntity(std::unique_ptr<Entity> entity);
        Entity* GetEntity(const std::string& name);

    protected:
        std::vector<std::unique_ptr<Entity>> m_Entities;
    };

}
