
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <vector>
#include <functional>

// Simulator
#include "core/compatibility.hpp"
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
class DLL_EXPORT Object
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
    explicit Object(Simulation& simulation, Type type = Type::Static) NOEXCEPT;


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
    Simulation& getSimulation() NOEXCEPT
    {
        return m_simulation;
    }


    /**
     * @brief Return simulation that owns object.
     *
     * @return
     */
    const Simulation& getSimulation() const NOEXCEPT
    {
        return m_simulation;
    }


    /**
     * @brief Returns object ID.
     *
     * @return
     */
    IdType getId() const NOEXCEPT
    {
        return m_id;
    }


    /**
     * @brief Returns object type.
     *
     * @return
     */
    Type getType() const NOEXCEPT;


    /**
     * @brief Returns current position.
     *
     * @return
     */
	core::PositionVector getPosition() const NOEXCEPT;


    /**
     * @brief Returns current rotation.
     *
     * @return
     */
	core::units::Angle getRotation() const NOEXCEPT;


    /**
     * @brief Returns current velocity.
     *
     * @return
     */
	core::VelocityVector getVelocity() const NOEXCEPT;


#if ENABLE_PHYSICS
    /**
     * @brief Returns physical body.
     *
     * @return
     */
    b2Body* getBody() const NOEXCEPT
    {
        return m_body;
    }
#endif


    /**
     * @brief Returns object shapes.
     *
     * @return
     */
    const std::vector<Shape>& getShapes() const NOEXCEPT
    {
        return m_shapes;
    }


    /**
     * @brief Returns object programs.
     *
     * @return
     */
    const std::vector<Program>& getPrograms() const NOEXCEPT
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
    void setType(Type type) NOEXCEPT;


    /**
     * @brief Change object position.
     *
     * @param pos
     */
	void setPosition(core::PositionVector pos) NOEXCEPT;


    /**
     * @brief Change object rotation.
     *
     * @param angle
     */
	void setRotation(core::units::Angle angle) NOEXCEPT;


    /**
     * @brief Change object velocity.
     *
     * @param vel
     */
	void setVelocity(core::VelocityVector vel) NOEXCEPT;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     */
	void applyForce(const core::ForceVector& force) NOEXCEPT;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     * @param pos
     */
	void applyForce(const core::ForceVector& force, const core::PositionVector& pos) NOEXCEPT;


    /**
     * @brief Set object shapes.
     *
     * @param shapes
     */
    void setShapes(std::vector<Shape> shapes) NOEXCEPT
    {
        m_shapes = std::move(shapes);
    }


    /**
     * @brief Set object programs.
     *
     * @param programs
     */
    void setPrograms(std::vector<Program> programs) NOEXCEPT
    {
        m_programs = std::move(programs);
    }


    /**
     * @brief Add a new program.
     *
     * @param program
     */
    void addProgram(Program program) NOEXCEPT
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
    bool is() const NOEXCEPT
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }


    /**
     * @brief Cast object into required type.
     *
     * @return
     */
    template<typename T>
    T* cast() NOEXCEPT
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
    const T* cast() const NOEXCEPT
    {
        assert(dynamic_cast<const T*>(this));
        return static_cast<const T*>(this);
    }


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
	virtual void update(core::units::Duration dt);


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    virtual void configure(const ConfigurationBase& config, Simulation& simulation);


#if ENABLE_RENDER
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
    std::vector<Shape>& getMutableShapes() NOEXCEPT
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
	core::PositionVector m_position;

    /// Object rotation.
	core::units::Angle m_rotation;

    /// Object velocity.
	core::VelocityVector m_velocity;
#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */
