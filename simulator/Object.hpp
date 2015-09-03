/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <functional>

// Simulator
#include "core/Units.hpp"
#include "core/Any.hpp"
#include "core/VectorUnits.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "simulator/Shape.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Program.hpp"

#ifdef ENABLE_RENDER
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
        /// Static body.
        Static,

        /// Dynamic body.
        Dynamic,

        /// Dynamic body pinned to initial position.
        Pinned
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
     * @brief Returns object dynamic values.
     *
     * @return
     */
    const Map<String, Any>& getValues() const noexcept
    {
        return m_values;
    }


    /**
     * @brief Check if dynamic value exists.
     *
     * @param name Value name.
     *
     * @return If value exists.
     */
    bool hasValue(const String& name) const noexcept
    {
        return m_values.find(name) != m_values.end();
    }


    /**
     * @brief Returns mutable object dynamic value.
     *
     * If value not exists yet, is created with default constructor.
     *
     * @param name Value name.
     *
     * @return Mutable reference to dynamic object.
     */
    Any& getValue(const String& name) noexcept
    {
        return m_values[name];
    }


    /**
     * @brief Returns object dynamic value.
     *
     * @param name Value name.
     * @param def  Default value if value doesn't exists.
     *
     * @return
     */
    Any getValue(const String& name, const Any& def = {}) const noexcept
    {
        auto it = m_values.find(name);
        return it != m_values.end() ? it->second : def;
    }


    /**
     * @brief Returns object dynamic value cast to custom type.
     *
     * @tparam T Output value type.
     *
     * @param name Value name.
     * @param def  Default value if value doesn't exists.
     *
     * @return Required value cast to required type.
     *
     * @throw BadCastException
     */
    template<typename T>
    T getValue(const String& name, T def = {}) const
    {
        return AnyCast<T>(getValue(name, Any(std::move(def))));
    }


    /**
     * @brief Returns object type.
     *
     * @return
     */
    Type getType() const noexcept
    {
        return m_type;
    }


#if ENABLE_RENDER
    /**
     * @brief Returns if object is visible.
     *
     * @return
     */
    bool isVisible() const noexcept
    {
        return m_visible;
    }
#endif


    /**
     * @brief Returns current object density.
     *
     * @return
     */
    units::Density getDensity() const noexcept
    {
        return m_density;
    }


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


    /**
     * @brief Returns current force.
     *
     * @return
     */
    const ForceVector& getForce() const noexcept
    {
        return m_force;
    }


    /**
     * @brief Returns object mass.
     *
     * @return
     */
    units::Mass getMass() const noexcept;


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
    const DynamicArray<Shape>& getShapes() const noexcept
    {
        return m_shapes;
    }


    /**
     * @brief Returns mutable object shapes.
     *
     * This version allows to update shapes regulary without allocating new
     * memory for shapes vector and then replace the old one with the new one.
     * Updating in place saves allocations and it's faster.
     *
     * @return
     */
    DynamicArray<Shape>& getMutableShapes() noexcept
    {
        return m_shapes;
    }


    /**
     * @brief Returns object programs.
     *
     * @return
     */
    const DynamicArray<Program>& getPrograms() const noexcept
    {
        return m_programs;
    }


// Public Mutators
public:


    /**
     * @brief Set object dynamic value.
     *
     * @tparam T Value type.
     *
     * @param name  Value name.
     * @param value Value.
     */
    template<typename T>
    void setValue(const String& name, T&& value) noexcept
    {
        m_values[name] = std::forward<T>(value);
    }


    /**
     * @brief Change object type.
     *
     * @return
     */
    void setType(Type type) noexcept;


#if ENABLE_RENDER
    /**
     * @brief Set if object is visible.
     *
     * @param flag
     */
    void setVisible(bool flag) noexcept
    {
        m_visible = flag;
    }
#endif


    /**
     * @brief Set object density.
     *
     * @param density New density value.
     */
    void setDensity(units::Density density) noexcept
    {
        m_density = density;
    }


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
     * @brief Set current force.
     *
     * @param force
     */
    void setForce(ForceVector force) noexcept
    {
        m_force = std::move(force);
    }


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
    void setShapes(DynamicArray<Shape> shapes) noexcept
    {
        m_shapes = std::move(shapes);
    }


    /**
     * @brief Set object programs.
     *
     * @param programs
     */
    void setPrograms(DynamicArray<Program> programs) noexcept
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
     * @brief Cast object into required type.
     *
     * @tparam T       Required result type.
     * @tparam Message Type of error message.
     *
     * @param msg Error message.
     *
     * @return Reference.
     */
    template<typename T, typename Message = const char*>
    T& castThrow(Message msg = "Invalid cast")
    {
        auto ptr = dynamic_cast<T*>(this);

        if (!ptr)
            throw InvalidCastException(msg);

        return *ptr;
    }


    /**
     * @brief Cast object into required type.
     *
     * @tparam T       Required result type.
     * @tparam Message Type of error message.
     *
     * @param msg Error message.
     *
     * @return Constant reference.
     */
    template<typename T, typename Message = const char*>
    const T& castThrow(Message msg = "Invalid cast") const
    {
        const auto ptr = dynamic_cast<const T*>(this);

        if (!ptr)
            throw InvalidCastException(msg);

        return *ptr;
    }


    /**
     * @brief Delete object.
     *
     * Queue object to be deleted/destroyed.
     */
    void destroy();


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
    virtual void configure(const Configuration& config, Simulation& simulation);


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


// Private Data Members
private:

    /// Owning simulation.
    Simulation& m_simulation;

    /// Object unique ID.
    IdType m_id;

    /// Object data.
    Map<String, Any> m_values;

    /// A list of object shapes.
    DynamicArray<Shape> m_shapes;

    /// Registered object programs.
    DynamicArray<Program> m_programs;

    /// Object density.
    units::Density m_density = units::Density(1); // FIXME: use better value

    /// Object type.
    Type m_type;

#if ENABLE_RENDER
    // If object is visible.
    bool m_visible = true;
#endif

#if ENABLE_PHYSICS
    /// Physics body.
    b2Body* m_body;

    /// Physics body for pin.
    b2Body* m_pinBody = nullptr;

    /// Joint for pinned body.
    b2Joint* m_pinJoint = nullptr;
#else
    /// Object position
    PositionVector m_position;

    /// Object rotation.
    units::Angle m_rotation;

    /// Object velocity.
    VelocityVector m_velocity;
#endif

    /// Box2D doesn't have accessor to force.
    ForceVector m_force;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
