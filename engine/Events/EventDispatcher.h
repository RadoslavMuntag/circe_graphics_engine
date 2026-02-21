#pragma once

#include "Event.h"

namespace Circe {

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& event)
            : m_Event(event) {
        }

        template<typename TEvent, typename TFunc>
        bool Dispatch(const TFunc& func) {
            if (m_Event.GetEventType() == TEvent::GetStaticType()) {
                m_Event.Handled = func(static_cast<TEvent&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

}
