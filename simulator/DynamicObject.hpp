
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/Velocity.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Dynamic simulation object with known position and velocity.
 */
class DynamicObject : public StaticObject
{

// Public Accessors
public:


    /**
     * @brief Returns current velocity.
     *
     * @return
     */
    const Velocity& getVelocity() const noexcept
    {
        return m_velocity;
    }


// Public Mutators
public:


    /**
     * @brief Change object velocity.
     *
     * @param vel
     */
    void setVelocity(Velocity vel) noexcept
    {
        m_velocity = vel;
    }


// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * @param dt Time step.
     */
    void update(units::Duration dt) override;


// Private Data Members
private:

    /// Object velocity.
    Velocity m_velocity;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
