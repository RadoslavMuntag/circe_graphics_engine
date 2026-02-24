#include "pch.h"

#include "Light.h"
#include "../Scene/Entity.h"

namespace Circe {

	Light::Light(LightType type)
		: m_Type(type) {
	}

	glm::vec3 Light::GetWorldPosition() const {
		if (const auto* owner = GetOwner()) {
			return owner->GetTransform().Position;
		}

		return m_LocalPosition;
	}

	glm::vec3 Light::GetWorldDirection() const {
		if (const auto* owner = GetOwner()) {
			return owner->GetTransform().Forward();
		}

		return glm::normalize(m_LocalDirection);
	}

}
