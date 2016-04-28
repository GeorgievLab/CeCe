/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <cassert>
#include <functional>

// CeCe
#include "cece/export.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Any.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/Shape.hpp"
#include "cece/core/Exception.hpp"
#include "cece/program/Program.hpp"
#include "cece/program/Container.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Color.hpp"
#  include "cece/render/Context.hpp"
#endif

// Box2D
#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  include "cece/simulator/ConverterBox2D.hpp"
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_BOX2D_PHYSICS
class b2Body;
class b2Shape;
class b2Joint;
#endif

namespace cece { namespace simulator { class Simulation; } }
namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

/**
 * @brief Basic simulation object.
 */
class CECE_EXPORT Object
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
     * @param typeName   Type name of the object.
     * @param type       Object type.
     */
    explicit Object(simulator::Simulation& simulation, String typeName = "simulator.Object", Type type = Type::Static) noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~Object();


// Public Accessors
public:


    /**
     * @brief Return object type name.
     *
     * @return
     */
    StringView getTypeName() const noexcept
    {
        return m_typeName;
    }


    /**
     * @brief Return simulation that owns object.
     *
     * @return
     */
    simulator::Simulation& getSimulation() noexcept
    {
        return m_simulation;
    }


    /**
     * @brief Return simulation that owns object.
     *
     * @return
     */
    const simulator::Simulation& getSimulation() const noexcept
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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns if object is visible.
     *
     * @return
     */
    bool isVisible() const noexcept
    {
        return m_visible;
    }


    /**
     * @brief Returns object color.
     *
     * @return
     */
    const render::Color& getColor() const noexcept
    {
        return m_color;
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
     * @brief Returns mass center world position.
     *
     * @return
     */
    PositionVector getMassCenterPosition() const noexcept;


    /**
     * @brief Returns mass center local position (offset).
     *
     * @return
     */
    PositionVector getMassCenterOffset() const noexcept;


    /**
     * @brief Convert local position to world position.
     *
     * @param local
     *
     * @return
     */
    PositionVector getWorldPosition(PositionVector local) const noexcept;


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
     * @brief Returns angular velocity.
     *
     * @return
     */
    units::AngularVelocity getAngularVelocity() const noexcept;


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


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Returns physical body.
     *
     * @return
     */
    b2Body* getBody() const noexcept
    {
        return m_body;
    }


    /**
     * @brief Returns Box2D units converter.
     *
     * @return
     */
    simulator::ConverterBox2D& getConverter() noexcept;


    /**
     * @brief Returns Box2D units converter.
     *
     * @return
     */
    const simulator::ConverterBox2D& getConverter() const noexcept;

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
    const program::Container& getPrograms() const noexcept
    {
        return m_programs;
    }


    /**
     * @brief Returns maximum translation vector magnitude per iteration.
     *
     * @return
     */
    units::Length getMaxTranslation() const noexcept;


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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Set if object is visible.
     *
     * @param flag
     */
    void setVisible(bool flag) noexcept
    {
        m_visible = flag;
    }


    /**
     * @brief Set object color.
     *
     * @param color
     */
    void setColor(const render::Color& color) noexcept
    {
        m_color = color;
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
     * @brief Change object angular velocity.
     *
     * @param vel
     */
    void setAngularVelocity(units::AngularVelocity vel) noexcept;


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
     * @param offset Local offset.
     */
    void applyForce(const ForceVector& force, const PositionVector& offset) noexcept;


    /**
     * @brief Push into object by given impulse.
     *
     * @param impulse
     */
    void applyLinearImpulse(const ImpulseVector& impulse) noexcept
    {
        applyLinearImpulse(impulse, getMassCenterOffset());
    }


    /**
     * @brief Push into object by given impulse.
     *
     * @param impulse
     * @param offset Local offset.
     */
    void applyLinearImpulse(const ImpulseVector& impulse, const PositionVector& offset) noexcept;


    /**
     * @brief Push into object by given impulse.
     *
     * @param impulse
     */
    void applyAngularImpulse(const units::Impulse& impulse) noexcept;


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
    void setPrograms(program::Container programs) noexcept
    {
        m_programs = std::move(programs);
    }


    /**
     * @brief Add a new program.
     *
     * @param program
     */
    void addProgram(UniquePtr<program::Program> program) noexcept
    {
        // Store program and init
        m_programs.add(std::move(program))->init(getSimulation(), *this);
    }


    /**
     * @brief Use program.
     *
     * It gets named program from simulation and bind it to this object.
     *
     * @param name Program name.
     */
    void useProgram(StringView name) noexcept;


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
    virtual void configure(const config::Configuration& config, simulator::Simulation& simulation);


    /**
     * @brief Initialize shapes for physics engine.
     */
    virtual void initShapes();


#ifdef CECE_ENABLE_RENDER

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
    simulator::Simulation& m_simulation;

    /// Object type name.
    String m_typeName;

    /// Object unique ID.
    IdType m_id;

    /// Object data.
    Map<String, Any> m_values;

    /// A list of object shapes.
    DynamicArray<Shape> m_shapes;

    /// Registered object programs.
    program::Container m_programs;

    /// Object density.
    units::Density m_density = units::Density(1); // FIXME: use better value

    /// Object type.
    Type m_type;

#ifdef CECE_ENABLE_RENDER

    // If object is visible.
    bool m_visible = true;

    /// Object color.
    render::Color m_color = render::colors::BLACK;

#endif

#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /// Physics body.
    b2Body* m_body;

    /// Physics body for pin.
    b2Body* m_pinBody = nullptr;

    /// Joint for pinned body.
    b2Joint* m_pinJoint = nullptr;

    /// Body shapes.
    DynamicArray<UniquePtr<b2Shape>> m_bodyShapes;

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

    /// Outstream for object data
    UniquePtr<OutStream> m_dataOut;

};

/* ************************************************************************ */

/**
 * @brief Read object type from stream.
 *
 * @param is   Input stream.
 * @param type
 *
 * @return is.
 */
inline InStream& operator>>(InStream& is, Object::Type& type)
{
    String value;
    is >> value;

    if (value == "static")
        type = object::Object::Type::Static;
    else if (value == "pinned")
        type = object::Object::Type::Pinned;
    else
        type = object::Object::Type::Dynamic; // Default

    return is;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
