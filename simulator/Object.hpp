
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <vector>

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


// Public Structures
public:


    /**
     * @brief Structure for circle shape.
     */
    struct ShapeCircle
    {
        /// Shape center.
        PositionVector center;

        /// Circle radius.
        units::Length radius;
    };


    /**
     * @brief Structure for storing shape.
     */
    struct Shape
    {
        /// Type of the shape.
        ShapeType type;

        union
        {
            ShapeCircle circle;
        };


        /**
         * @brief Create circle shape.
         *
         * @param radius Circle radius.
         * @param center Circle center (position offset).
         *
         * @return
         */
        static Shape makeCircle(units::Length radius, PositionVector center = {0, 0}) noexcept
        {
            return Shape{ShapeType::Circle, ShapeCircle{center, radius}};
        }
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


    /**
     * @brief Returns object shapes.
     *
     * @return
     */
    const std::vector<Shape>& getShapes() const noexcept
    {
        return m_shapes;
    }


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
    void applyForce(const ForceVector& force) noexcept;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     * @param pos
     */
    void applyForce(const ForceVector& force, const PositionVector& pos) noexcept;


    /**
     * @brief Set object shapes.
     *
     * @param shapes
     */
    void setShapes(std::vector<Shape> shapes) noexcept
    {
        m_shapes = std::move(shapes);
    }


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


// Protected Accessors
protected:


    /**
     * @brief Returns mutable object shapes.
     *
     * This version allows to update shapes regulary without allocating new
     * memory for shapes vector and then replace the old one with the new one.
     * Updating in place saves allocations and it's faster.
     *
     * @return
     */
    std::vector<Shape>& getShapes() noexcept
    {
        return m_shapes;
    }


// Private Data Members
private:

    /// Owning simulation.
    Simulation& m_simulation;

    /// Object unique ID.
    IdType m_id;

    /// A list of object shapes.
    std::vector<Shape> m_shapes;

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
