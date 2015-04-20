
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/Velocity.hpp"
#include "simulator/Acceleration.hpp"

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
     *
     * @param simulation
     */
    explicit DynamicObject(Simulation& simulation) noexcept
        : StaticObject(simulation)
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


    /**
     * @brief Returns current acceleration.
     *
     * @return
     */
    const Acceleration& getAcceleration() const noexcept
    {
        return m_acceleration;
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


    /**
     * @brief Change object acceleration.
     *
     * @param acc
     */
    void setAcceleration(Acceleration acc) noexcept
    {
        m_acceleration = std::move(acc);
    }


    /**
     * @brief Add object acceleration.
     *
     * @param acc
     */
    void addAcceleration(const Acceleration& acc) noexcept
    {
        m_acceleration += acc;
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

    /// Object acceleration.
    Acceleration m_acceleration{};

};

/* ************************************************************************ */

}

/* ************************************************************************ */
