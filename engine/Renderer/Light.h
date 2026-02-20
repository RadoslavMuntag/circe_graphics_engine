#pragma once

#include <glm/glm.hpp>
#include "../Scene/Behavior.h"

namespace Circe {

	enum class LightType {
		Directional,
		Point,
		Spot
	};

	class Light : public Behavior {
	public:
		explicit Light(LightType type = LightType::Point);

		LightType GetType() const { return m_Type; }
		void SetType(LightType type) { m_Type = type; }

		void SetColor(const glm::vec3& color) { m_Color = color; }
		glm::vec3 GetColor() const { return m_Color; }

		void SetIntensity(float intensity) { m_Intensity = intensity; }
		float GetIntensity() const { return m_Intensity; }

		void SetRange(float range) { m_Range = range; }
		float GetRange() const { return m_Range; }

		void SetInnerAngle(float radians) { m_InnerAngle = radians; }
		float GetInnerAngle() const { return m_InnerAngle; }

		void SetOuterAngle(float radians) { m_OuterAngle = radians; }
		float GetOuterAngle() const { return m_OuterAngle; }

		void SetLocalPosition(const glm::vec3& position) { m_LocalPosition = position; }
		glm::vec3 GetWorldPosition() const;

		void SetLocalDirection(const glm::vec3& direction) { m_LocalDirection = direction; }
		glm::vec3 GetWorldDirection() const;

	private:
		LightType m_Type = LightType::Point;
		glm::vec3 m_Color = glm::vec3(1.0f);
		float m_Intensity = 1.0f;
		float m_Range = 10.0f;
		float m_InnerAngle = 0.261799f;
		float m_OuterAngle = 0.436332f;
		glm::vec3 m_LocalPosition = glm::vec3(0.0f);
		glm::vec3 m_LocalDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	};

}
