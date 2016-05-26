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

// C++
#include <cstdlib>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/String.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/Map.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/streamlines/Dynamics.hpp"
#include "cece/plugins/streamlines/Lattice.hpp"
#include "cece/plugins/streamlines/Converter.hpp"
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Returns ranges based on boundary position.
 *
 * @param position
 * @param size
 *
 * @return
 */
StaticArray<Vector<Lattice::SizeType>, 2>
getRanges(Boundary::Position position, const Lattice::Size& size)
{
    switch (position)
    {
    case Boundary::Position::Top:
        return {{
            {0, size.getHeight() - 1},
            {size.getWidth(), size.getHeight()}
        }};

    case Boundary::Position::Bottom:
        return {{
            {0, 0},
            {size.getWidth(), 1}
        }};

    case Boundary::Position::Right:
        return {{
            {size.getWidth() - 1, 0},
            {size.getWidth(), size.getHeight()}
        }};

    case Boundary::Position::Left:
        return {{
            {0, 0},
            {1, size.getHeight()}
        }};

    default:
        Assert(false && "Invalid boundary position");
        std::abort();
    }
}

/* ************************************************************************ */

/**
 * @brief Returns ranges based on boundary position.
 *
 * @param position
 * @param size
 *
 * @return
 */
StaticArray<Vector<Lattice::SizeType>, 2>
getRanges(Boundary::Position position, RealType offset, RealType size,
    const Lattice::Size& gridSize)
{
    switch (position)
    {
    case Boundary::Position::Top:
    {
        Lattice::SizeType off = gridSize.getWidth() / 2 + offset;
        Lattice::SizeType siz = size / 2;

        return {{
            {off - siz, gridSize.getHeight() - 1},
            {off + siz, gridSize.getHeight()}
        }};
    }

    case Boundary::Position::Bottom:
    {
        Lattice::SizeType off = gridSize.getWidth() / 2 + offset;
        Lattice::SizeType siz = size / 2;

        return {{
            {off - siz, 0},
            {off + siz, 1}
        }};
    }

    case Boundary::Position::Right:
    {
        Lattice::SizeType off = gridSize.getHeight() / 2;
        Lattice::SizeType siz = size / 2;

        return {{
            {gridSize.getWidth() - 1, off - siz},
            {gridSize.getWidth(), off + siz}
        }};
    }

    case Boundary::Position::Left:
    {
        Lattice::SizeType off = gridSize.getHeight() / 2;
        Lattice::SizeType siz = size / 2;

        return {{
            {0, off - siz},
            {1, off + siz}
        }};
    }

    default:
        Assert(false && "Invalid boundary position");
        std::abort();
    }
}

/* ************************************************************************ */

}

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
    setOffset(config.get("offset", getOffset()));
    setSize(config.get("size", getSize()));
    setInletProfileType(config.get("inlet-profile", getInletProfileType()));
    setInletVelocity(config.get("inlet-velocity", getInletVelocity()));
}

/* ************************************************************************ */

void Boundary::storeConfig(config::Configuration& config) const
{
    config.set("type", getType());
    config.set("position", getPosition());
    config.get("offset", getOffset());
    config.get("size", getSize());
    config.set("inlet-profile", getInletProfileType());
    config.set("inlet-velocity", getInletVelocity());
}

/* ************************************************************************ */

void Boundary::apply(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics)
{
    const auto gridSize = lattice.getSize();

    // Get ranges
    auto ranges = m_size == Zero
        ? getRanges(m_position, gridSize)
        : getRanges(m_position, converter.convertLength(m_offset), converter.convertLength(m_size), gridSize)
    ;

    const Vector<Lattice::SizeType> rngMin = ranges[0];
    const Vector<Lattice::SizeType> rngMax = ranges[1];

    if (m_type == Type::Outlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            auto& node = lattice[{x, y}];

            // Set boundary dynamics
            if (node.getDynamics() == fluidDynamics)
                node.setDynamics(m_dynamics);

            if (node.getDynamics() == m_dynamics)
                node.defineDensity(1.0);
        }
    }
    else if (m_type == Type::Inlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            auto& node = lattice[{x, y}];

            // Set boundary dynamics
            if (node.getDynamics() == fluidDynamics)
                node.setDynamics(m_dynamics);

            if (node.getDynamics() == m_dynamics)
                node.defineVelocity(converter.convertVelocity(inletVelocity({x, y})));
        }
    }
}

/* ************************************************************************ */

VelocityVector Boundary::inletVelocity(Lattice::CoordinateType coord) const noexcept
{
    auto type = getInletProfileType();

    if (type == InletProfileType::Auto)
        type = InletProfileType::Constant;

    if (type == InletProfileType::Constant)
    {
        switch (m_position)
        {
        case Position::Top:
            return {Zero, -m_inletVelocity};

        case Position::Bottom:
            return {Zero, m_inletVelocity};

        case Position::Left:
            return {m_inletVelocity, Zero};

        case Position::Right:
            return {-m_inletVelocity, Zero};

        default:
            Assert(false && "Invalid boundary position");
            std::abort();
        }
    }

    return Zero;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
