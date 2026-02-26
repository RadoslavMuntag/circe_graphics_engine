#pragma once

#include "../Core/Export.h"
#include "../Events/Event.h"

namespace Circe {

    class Entity;

    class CIRCE_API Behavior {
    public:
        virtual ~Behavior() = default;
        
        void SetOwner(Entity* owner) { m_Owner = owner; }
        Entity* GetOwner() const { return m_Owner; }

        template<typename TOwner>
        TOwner* GetOwnerAs() const {
            return dynamic_cast<TOwner*>(m_Owner); // requires polymorphic base
        }

        virtual void OnInit() {}
        virtual void OnUpdate(float deltaTime) { (void)deltaTime; }
        virtual void OnRender() {}
        virtual void OnEvent(Event& event) { (void)event; }
        
    private:
        Entity* m_Owner = nullptr;
    };
}