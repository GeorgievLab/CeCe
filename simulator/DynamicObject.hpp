
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


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    DynamicObject()
    {
        setFlag(OBJECT_DYNAMIC);
    }


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
        m_velocity = std::move(vel);
    }


// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * Function applies object velocity to position.
     *
     * @param dt Simulation time step.
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
