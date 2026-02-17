#pragma once

namespace Circe {

    class Entity;

    class Behavior {
    public:
        virtual ~Behavior() = default;
        
        void SetOwner(Entity* owner) { m_Owner = owner; }
        Entity* GetOwner() const { return m_Owner; }

        virtual void OnInit() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        
    private:
        Entity* m_Owner = nullptr;
    };
}