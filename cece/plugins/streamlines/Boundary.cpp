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

// Declaration
#include "cece/plugins/streamlines/Boundary.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/String.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Read boundary type.
 *
 * @param is   Input stream.
 * @param type Output variable.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Boundary::Type& type)
{
    String desc;
    is >> desc;

    if (desc == "none")
        type = Boundary::Type::None;
    else if (desc == "barrier")
        type = Boundary::Type::Barrier;
    else if (desc == "inlet")
        type = Boundary::Type::Inlet;
    else if (desc == "outlet")
        type = Boundary::Type::Outlet;
    else
        throw InvalidArgumentException("Unknown boundary type");

    return is;
}

/* ************************************************************************ */

/**
 * @brief Write boundary type.
 *
 * @param os   Output stream.
 * @param type Boundary type.
 *
 * @return os.
 */
OutStream& operator<<(OutStream& os, const Boundary::Type& type)
{
    switch (type)
    {
    case Boundary::Type::None:      os << "none"; break;
    case Boundary::Type::Barrier:   os << "barrier"; break;
    case Boundary::Type::Inlet:     os << "inlet"; break;
    case Boundary::Type::Outlet:    os << "outlet"; break;
    default: Assert(false && "Unknown boundary type");
    }

    return os;
}

/* ************************************************************************ */

/**
 * @brief Read boundary position.
 *
 * @param is       Input stream.
 * @param position Output variable.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Boundary::Position& position)
{
    String desc;
    is >> desc;

    if (desc == "left")
        position = Boundary::Position::Left;
    else if (desc == "right")
        position = Boundary::Position::Right;
    else if (desc == "top")
        position = Boundary::Position::Top;
    else if (desc == "bottom")
        position = Boundary::Position::Bottom;
    else
        throw InvalidArgumentException("Unknown boundary position");

    return is;
}

/* ************************************************************************ */

/**
 * @brief Write boundary position.
 *
 * @param os       Output stream.
 * @param position Boundary position.
 *
 * @return os.
 */
OutStream& operator<<(OutStream& os, const Boundary::Position& position)
{
    switch (position)
    {
    case Boundary::Position::Right:     os << "right"; break;
    case Boundary::Position::Left:      os << "left"; break;
    case Boundary::Position::Top:       os << "top"; break;
    case Boundary::Position::Bottom:    os << "bottom"; break;
    default: Assert(false && "Unknown boundary position");
    }

    return os;
}

/* ************************************************************************ */

/**
 * @brief Read boundary inlet velocity profile.
 *
 * @param is
 * @param type
 *
 * @return
 */
InStream& operator>>(InStream& is, Boundary::InletProfileType& type)
{
    String desc;
    is >> desc;

    if (desc == "constant")
        type = Boundary::InletProfileType::Constant;
    else if (desc == "parabolic")
        type = Boundary::InletProfileType::Parabolic;
    else if (desc == "auto")
        type = Boundary::InletProfileType::Auto;
    else
        throw InvalidArgumentException("Unknown inlet velocity profile type");

    return is;
}

/* ************************************************************************ */

/**
 * @brief Write inlet velocity type into stream.
 *
 * @param os   Output stream.
 * @param type Profile type.
 *
 * @return
 */
OutStream& operator<<(OutStream& os, const Boundary::InletProfileType& type)
{
    switch (type)
    {
    case Boundary::InletProfileType::Auto:
        os << "auto"; break;
    case Boundary::InletProfileType::Constant:
        os << "constant"; break;
    case Boundary::InletProfileType::Parabolic:
        os << "parabolic"; break;
    default:
        Assert(false && "Unknown inlet velocity profile type");
    }
    return os;
}

/* ************************************************************************ */

void Boundary::loadConfig(const config::Configuration& config)
{
    setType(config.get("type", getType()));
    setPosition(config.get("position", getPosition()));
    setCenter(config.get("center", getCenter()));
    setSize(config.get("size", getSize()));
    setInletProfileType(config.get("inlet-profile", getInletProfileType()));
}

/* ************************************************************************ */

void Boundary::storeConfig(config::Configuration& config) const
{
    config.set("type", getType());
    config.set("position", getPosition());
    config.set("center", getCenter());
    config.set("size", getSize());
    config.set("inlet-profile", getInletProfileType());
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
