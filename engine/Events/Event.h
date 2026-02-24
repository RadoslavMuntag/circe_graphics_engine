#pragma once

#include "Core/Export.h"
#include <string>

namespace Circe {

	enum class EventType {
		None = 0,
		//Application events
		WindowClose,
		WindowResize,
		//Input events
		KeyPressed,
		KeyReleased,
		KeyTyped,
		//Mouse events
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory {
		EventCategoryNone = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
							  EventType GetEventType() const override { return GetStaticType(); }\
							  const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) int GetCategoryFlags() const override { return category; }

	class CIRCE_API Event {
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category) const {
			return (GetCategoryFlags() & category) != 0;
		}
	};

}