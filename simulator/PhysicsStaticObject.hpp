
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/Velocity.hpp"

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Physics static object.
 */
class PhysicsStaticObject : public StaticObject
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit PhysicsStaticObject(Simulation& simulation) noexcept;


#ifdef ENABLE_PHYSICS
    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param body
     */
    PhysicsStaticObject(Simulation& simulation, b2Body* body) noexcept;
#endif


    /**
     * @brief Destructor.
     */
    ~PhysicsStaticObject();


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


// Public Mutators
public:


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(Position pos) noexcept override;


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
