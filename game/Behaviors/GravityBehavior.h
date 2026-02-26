#pragma once

#include <Circe.h>

namespace Game{
    
    class GravityBehavior : public Circe::Behavior {
    public:
        GravityBehavior(float mass = 1.0f);

        void OnUpdate(float dt) override ;
        void ApplyForce(const glm::vec3& force) ;

        void SetVelocity(const glm::vec3& velocity) ;

        float GetMass() const { return m_Mass; }
        glm::vec3 GetVelocity() const { return m_Velocity; }

    private:
        float m_Mass;
        glm::vec3 m_Velocity;
        glm::vec3 m_Acceleration;
    };
}