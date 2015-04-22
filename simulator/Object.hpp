
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

#if ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Basic simulation object.
 */
class Object
{

// Public Enums
public:


    /**
     * @brief Object types.
     */
    enum class Type
    {
        Static,
        Dynamic
    };


    /**
     * @brief Shape types.
     */
    enum class ShapeType
    {
        Circle
    };


// Public Types
public:


    /// Object ID type.
    using IdType = unsigned long;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation Object owner.
     * @param type       Object type.
     */
    explicit Object(Simulation& simulation, Type type = Type::Static) noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~Object();


// Public Accessors
public:


    /**
     * @brief Return simulation that owns object.
     *
     * @return
     */
    Simulation& getSimulation() noexcept
    {
        return m_simulation;
    }


    /**
     * @brief Return simulation that owns object.
     *
     * @return
     */
    const Simulation& getSimulation() const noexcept
    {
        return m_simulation;
    }


    /**
     * @brief Returns object ID.
     *
     * @return
     */
    IdType getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Returns object type.
     *
     * @return
     */
    Type getType() const noexcept;


    /**
     * @brief Returns current position.
     *
     * @return
     */
    PositionVector getPosition() const noexcept;


    /**
     * @brief Returns current velocity.
     *
     * @return
     */
    VelocityVector getVelocity() const noexcept;


#if ENABLE_PHYSICS
    /**
     * @brief Returns physical body.
     *
     * @return
     */
    b2Body* getBody() const noexcept
    {
        return m_body;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Change object type.
     *
     * @return
     */
    void setType(Type type) noexcept;


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(PositionVector pos) noexcept;


    /**
     * @brief Change object velocity.
     *
     * @param vel
     */
    void setVelocity(VelocityVector vel) noexcept;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     */
    void applyForce(ForceVector force) noexcept;


// Public Operations
public:


    /**
     * @brief If object is given type.
     *
     * @return
     */
    template<typename T>
    bool is() const noexcept
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }


    /**
     * @brief Cast object into required type.
     *
     * @return
     */
    template<typename T>
    T* cast() noexcept
    {
        assert(dynamic_cast<T*>(this));
        return static_cast<T*>(this);
    }


    /**
     * @brief Cast object into required type.
     *
     * @return
     */
    template<typename T>
    const T* cast() const noexcept
    {
        assert(dynamic_cast<const T*>(this));
        return static_cast<const T*>(this);
    }


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    virtual void update(units::Duration dt);


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize object for rendering.
     *
     * @param context
     */
    virtual void drawInit(render::Context& context)
    {
        // Nothing to do
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render object.
     *
     * @param context
     */
    virtual void draw(render::Context& context)
    {
        // Nothing to do
    }
#endif

// Private Data Members
private:

    /// Owning simulation.
    Simulation& m_simulation;

    /// Object unique ID.
    IdType m_id;

#if ENABLE_PHYSICS
    /// Physics body.
    b2Body* m_body;
#else
    /// Object type.
    Type m_type;

    /// Object position
    PositionVector m_position;

    /// Object velocity.
    VelocityVector m_velocity;
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */
