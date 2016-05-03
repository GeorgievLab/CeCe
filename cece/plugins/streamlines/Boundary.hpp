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

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

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
        Right  = 0,
        Left   = 1,
        Top    = 2,
        Bottom = 3
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
     * @brief Returns boundary center position.
     *
     * @return
     */
    units::Length getCenter() const noexcept
    {
        return m_center;
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
     * @brief Set boundary center position.
     *
     * @param center
     */
    void setCenter(units::Length center) noexcept
    {
        m_center = center;
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


// Public Operations
private:


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


// Private Data Members
private:

    /// Boundary type
    Type m_type = Type::None;

    /// Boundary position.
    Position m_position;

    /// Boundary center position.
    units::Length m_center;

    /// Boundary size.
    units::Length m_size;

    /// Inlet velocity profile type.
    InletProfileType m_inletProfileType = InletProfileType::Auto;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
