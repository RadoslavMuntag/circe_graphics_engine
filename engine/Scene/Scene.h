#pragma once

#include "../Core/Export.h"
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
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender(Renderer& renderer) {}

        void Update(float deltaTime);
        void Render(Renderer& renderer);

        void AddEntity(std::unique_ptr<Entity> entity);
        Entity* GetEntity(const std::string& name);

    protected:
        std::vector<std::unique_ptr<Entity>> m_Entities;
    };

}
