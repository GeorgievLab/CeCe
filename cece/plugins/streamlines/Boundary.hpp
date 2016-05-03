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

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"

// Plugin
#include "cece/plugins/streamlines/Lattice.hpp"

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }
namespace cece { namespace object { class Object; } }
namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

class Dynamics;
class Converter;

/* ************************************************************************ */

/**
 * @brief Streamlines boundary specification.
 */
class Boundary
{

// Public Enums
public:


    /**
     * @brief Boundary type.
     */
    enum class Type
    {
        None,
        Barrier,
        Inlet,
        Outlet
    };


    /**
     * @brief Boundary position.
     */
    enum class Position
    {
        Right  = 1,
        Left   = 3,
        Top    = 0,
        Bottom = 2
    };


    /**
     * @brief Inlet velocity profile type.
     */
    enum class InletProfileType
    {
        Auto,
        Constant,
        Parabolic
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Boundary();


    /**
     * @brief Destructor.
     */
    ~Boundary();


// Public Accessors
public:


    /**
     * @brief Returns boundary type.
     *
     * @return
     */
    Type getType() const noexcept
    {
        return m_type;
    }


    /**
     * @brief Returns boundary position.
     *
     * @return
     */
    Position getPosition() const noexcept
    {
        return m_position;
    }


    /**
     * @brief Returns boundary type.
     *
     * @return
     */
    InletProfileType getInletProfileType() const noexcept
    {
        return m_inletProfileType;
    }


    /**
     * @brief Returns inlet velocity.
     *
     * @return
     */
    units::Velocity getInletVelocity() const noexcept
    {
        return m_inletVelocity;
    }


    /**
     * @brief Returns dynamics.
     *
     * @return
     */
    ViewPtr<Dynamics> getDynamics() const noexcept
    {
        return m_dynamics;
    }


    /**
     * @brief Returns barrier object.
     *
     * @return
     */
    ViewPtr<object::Object> getBarrierObject() const noexcept
    {
        return m_barrierObject;
    }


// Public Mutators
public:


    /**
     * @brief Set boundary type.
     *
     * @param type
     */
    void setType(Type type) noexcept
    {
        m_type = type;
    }


    /**
     * @brief Set boundary position.
     *
     * @param position
     */
    void setPosition(Position position) noexcept
    {
        m_position = position;
    }


    /**
     * @brief Set boundary type.
     *
     * @param type
     */
    void setInletProfileType(InletProfileType type) noexcept
    {
        m_inletProfileType = type;
    }


    /**
     * @brief Set inlet velocity.
     *
     * @param velocity
     */
    void setInletVelocity(units::Velocity velocity) noexcept
    {
        m_inletVelocity = velocity;
    }


    /**
     * @brief Set dynamics.
     *
     * @param dynamics
     */
    void setDynamics(UniquePtr<Dynamics> dynamics) noexcept
    {
        m_dynamics = std::move(dynamics);
    }


    /**
     * @brief Set barrier object.
     *
     * @param object
     */
    void setBarrierObject(ViewPtr<object::Object> object) noexcept
    {
        m_barrierObject = object;
    }


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config);


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const;


    /**
     * @brief Init barrier object.
     *
     * @param simulation
     */
    void initBarrier(simulator::Simulation& simulation);


    /**
     * @brief Initialize inlet or outlet.
     *
     * @param converter
     * @param lattice
     * @param fluidDynamics
     */
    void initInletOutlet(Converter& converter, Lattice& lattice, ViewPtr<Dynamics> fluidDynamics);


    /**
     * @brief Find fluid holes within lattice where is fluid dynamics.
     *
     * @param lattice
     * @param fluidDynamics
     */
    void findHoles(Lattice& lattice, ViewPtr<Dynamics> fluidDynamics);


    /**
     * @brief Calculate inlet velocity profile.
     *
     * @param coord
     *
     * @return
     */
    VelocityVector inletVelocity(Lattice::CoordinateType coord) const noexcept;


// Private Data Members
private:

    /// Boundary type
    Type m_type = Type::None;

    /// Boundary position.
    Position m_position;

    /// Inlet velocity profile type.
    InletProfileType m_inletProfileType = InletProfileType::Auto;

    /// Inlet velocity.
    units::Velocity m_inletVelocity = Zero;

    /// Boundary dynamics.
    UniquePtr<Dynamics> m_dynamics;

    /// Barrier object.
    ViewPtr<object::Object> m_barrierObject = nullptr;

    /// A list of boundary holes.
    DynamicArray<StaticArray<Lattice::CoordinateType, 2>> m_holes;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
