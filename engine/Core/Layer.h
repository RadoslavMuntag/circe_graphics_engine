#pragma once

#include "Export.h"
#include "../Events/Event.h"

namespace Circe {

    class CIRCE_API Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_Name; }

    protected:
        std::string m_Name; // Used for debugging
    };
}