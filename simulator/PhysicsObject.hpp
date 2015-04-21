
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/DynamicObject.hpp"
#include "simulator/Velocity.hpp"

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Physics object.
 */
class PhysicsObject : public DynamicObject
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit PhysicsObject(Simulation& simulation) noexcept;


#ifdef ENABLE_PHYSICS
    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param body
     */
    PhysicsObject(Simulation& simulation, b2Body* body) noexcept;
#endif


    /**
     * @brief Destructor.
     */
    ~PhysicsObject();


// Public Accessors
public:


#ifdef ENABLE_PHYSICS
    b2Body* getBody() const noexcept
    {
        return m_body;
    }
#endif


    /**
     * @brief Returns current position.
     *
     * @return
     */
    const Position& getPosition() const noexcept override;


    /**
     * @brief Returns current velocity.
     *
     * @return
     */
    const Velocity& getVelocity() const noexcept override;


// Public Mutators
public:


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(Position pos) noexcept override;


    /**
     * @brief Change object velocity.
     *
     * @param vel
     */
    void setVelocity(Velocity vel) noexcept override;


    /**
     * @brief Add object acceleration.
     *
     * @param acc
     */
    void addAcceleration(const Acceleration& acc) noexcept override;


// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    void update(units::Duration dt) override;


// Private Data Members
private:

#ifdef ENABLE_PHYSICS
    b2Body* m_body;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
