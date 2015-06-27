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
#include "core/compatibility.hpp"
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
     * @brief Returns object dynamic values.
     *
     * @return
     */
    const Map<String, Any>& getValues() const NOEXCEPT
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
    bool hasValue(const String& name) const NOEXCEPT
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
    Any& getValue(const String& name) NOEXCEPT
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
    Any getValue(const String& name, const Any& def = {}) const NOEXCEPT
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
    Type getType() const NOEXCEPT;


    /**
     * @brief Returns current object density.
     *
     * @return
     */
    units::Density getDensity() const NOEXCEPT
    {
        return m_density;
    }


    /**
     * @brief Returns current position.
     *
     * @return
     */
    PositionVector getPosition() const NOEXCEPT;


    /**
     * @brief Returns current rotation.
     *
     * @return
     */
    units::Angle getRotation() const NOEXCEPT;


    /**
     * @brief Returns current velocity.
     *
     * @return
     */
    VelocityVector getVelocity() const NOEXCEPT;


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
    const DynamicArray<Shape>& getShapes() const NOEXCEPT
    {
        return m_shapes;
    }


    /**
     * @brief Returns object programs.
     *
     * @return
     */
    const DynamicArray<Program>& getPrograms() const NOEXCEPT
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
    void setValue(const String& name, T&& value) NOEXCEPT
    {
        m_values[name] = std::forward<T>(value);
    }


    /**
     * @brief Change object type.
     *
     * @return
     */
    void setType(Type type) NOEXCEPT;


    /**
     * @brief Set object density.
     *
     * @param density New density value.
     */
    void setDensity(units::Density density) NOEXCEPT
    {
        m_density = density;
    }


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(PositionVector pos) NOEXCEPT;


    /**
     * @brief Change object rotation.
     *
     * @param angle
     */
    void setRotation(units::Angle angle) NOEXCEPT;


    /**
     * @brief Change object velocity.
     *
     * @param vel
     */
    void setVelocity(VelocityVector vel) NOEXCEPT;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     */
    void applyForce(const ForceVector& force) NOEXCEPT;


    /**
     * @brief Push into object by given force.
     *
     * @param force
     * @param pos
     */
    void applyForce(const ForceVector& force, const PositionVector& pos) NOEXCEPT;


    /**
     * @brief Set object shapes.
     *
     * @param shapes
     */
    void setShapes(DynamicArray<Shape> shapes) NOEXCEPT
    {
        m_shapes = std::move(shapes);
    }


    /**
     * @brief Set object programs.
     *
     * @param programs
     */
    void setPrograms(DynamicArray<Program> programs) NOEXCEPT
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
    DynamicArray<Shape>& getMutableShapes() NOEXCEPT
    {
        return m_shapes;
    }


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
