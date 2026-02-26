#include "GravityBehavior.h"

namespace Game {

    GravityBehavior::GravityBehavior(float mass)
        : m_Mass(mass), m_Velocity(0.0f), m_Acceleration(0.0f) {}

    void GravityBehavior::OnUpdate(float dt) {
        if (!GetOwner()) return;

        auto& transform = GetOwner()->GetTransform();

        // Apply acceleration to velocity
        m_Velocity += m_Acceleration * dt;

        // Apply velocity to position
        transform.Position += m_Velocity * dt;

        // Reset acceleration for next frame
        m_Acceleration = glm::vec3(0.0f);
    }

    void GravityBehavior::ApplyForce(const glm::vec3& force) {
        m_Acceleration += force / m_Mass;
    }

    void GravityBehavior::SetVelocity(const glm::vec3& velocity) {
        m_Velocity = velocity;
    }
    
}