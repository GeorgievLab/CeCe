
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <vector>
#include <functional>

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "simulator/Shape.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Program.hpp"

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
     * @brief Returns current rotation.
     *
     * @return
     */
    units::Angle getRotation() const noexcept;


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


    /**
     * @brief Returns object programs.
     *
     * @return
     */
    const std::vector<Program>& getPrograms() const noexcept
    {
        return m_programs;
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
     * @brief Change object rotation.
     *
     * @param angle
     */
    void setRotation(units::Angle angle) noexcept;


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


    /**
     * @brief Set object programs.
     *
     * @param programs
     */
    void setPrograms(std::vector<Program> programs) noexcept
    {
        m_programs = std::move(programs);
    }


    /**
     * @brief Add a new program.
     *
     * @param program
     */
    void addProgram(Program program) noexcept
    {
        m_programs.push_back(std::move(program));
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


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    virtual void configure(const ConfigurationBase& config, Simulation& simulation)
    {
        // Nothing to do
    }


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
    std::vector<Shape>& getMutableShapes() noexcept
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

    /// Registered object programs.
    std::vector<Program> m_programs;

#if ENABLE_PHYSICS
    /// Physics body.
    b2Body* m_body;
#else
    /// Object type.
    Type m_type;

    /// Object position
    PositionVector m_position;

    /// Object rotation.
    units::Angle m_rotation;

    /// Object velocity.
    VelocityVector m_velocity;
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */
