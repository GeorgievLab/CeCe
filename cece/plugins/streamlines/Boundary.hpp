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
#include "cece/core/String.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Range.hpp"
#include "cece/core/DynamicArray.hpp"

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
        Constant
    };


// Public Accessors
public:


    /**
     * @brief Returns boundary name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


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
     * @brief Returns boundary offset.
     *
     * @return
     */
    units::Length getOffset() const noexcept
    {
        return m_offset;
    }


    /**
     * @brief Returns boundary size.
     *
     * @return
     */
    units::Length getSize() const noexcept
    {
        return m_size;
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
     * @brief Returns inlet volumeric flow rate.
     *
     * @return
     */
    units::VolumericFlow getInletFlow() const noexcept
    {
        return m_inletFlow;
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
     * @brief Returns location blocks.
     *
     * @return
     */
    const DynamicArray<Range<Lattice::SizeType>>& getBlocks() const noexcept
    {
        return m_blocks;
    }


// Public Mutators
public:


    /**
     * @brief Set boundary name.
     *
     * @param name
     */
    void setName(String name) noexcept
    {
        m_name = std::move(name);
    }


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
     * @brief Set boundary offset.
     *
     * @param offset
     */
    void setOffset(units::Length offset) noexcept
    {
        m_offset = offset;
    }


    /**
     * @brief Set boundary size.
     *
     * @param size
     */
    void setSize(units::Length size) noexcept
    {
        m_size = size;
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
     * @brief Set inlet volumeric flow rate.
     *
     * @param flow
     */
    void setInletFlow(units::VolumericFlow flow) noexcept
    {
        m_inletFlow = flow;
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
     * @brief Update blocks that will be used as boundary.
     *
     * @param lattice
     * @param converter
     * @param fluidDynamics
     */
    void updateBlocks(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics);


    /**
     * @brief Apply boundary to lattice.
     *
     * @param lattice
     * @param converter
     * @param fluidDynamics
     */
    void apply(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics);


    /**
     * @brief Calculate inlet velocity profile.
     *
     * @param converter
     * @param coord
     * @param width Inlet width.
     *
     * @return
     */
    units::VelocityVector inletVelocity(Converter& converter, Lattice::CoordinateType coord, Lattice::SizeType width) const noexcept;


// Private Data Members
private:

    /// Boundary name.
    String m_name;

    /// Boundary type.
    Type m_type = Type::None;

    /// Boundary position.
    Position m_position;

    /// Boundary position offset.
    units::Length m_offset = Zero;

    /// Boundary size.
    units::Length m_size = Zero;

    /// Inlet velocity profile type.
    InletProfileType m_inletProfileType = InletProfileType::Auto;

    /// Inlet velocity.
    units::Velocity m_inletVelocity = Zero;

    /// Inlet volumeric flow rate.
    units::VolumericFlow m_inletFlow = Zero;

    /// Boundary dynamics.
    UniquePtr<Dynamics> m_dynamics;

    /// Boundary open blocks.
    DynamicArray<Range<Lattice::SizeType>> m_blocks;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
