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
 * @brief Calculare edge block.
 *
 * @param offset Center offset.
 * @param size   Boundary size.
 * @param count  Total lattice size.
 *
 * @return
 */
Range<Lattice::SizeType> getBlock(Lattice::SizeType offset, Lattice::SizeType size, Lattice::SizeType count) noexcept
{
    if (size == 0)
        size = count;

    const Lattice::SizeType off = count / 2 + offset;
    const Lattice::SizeType siz = size / 2;

    return {off - siz, off + siz};
}

/* ************************************************************************ */

/**
 * @brief Detect blocks for boundary.
 *
 * @param lattice
 * @param range
 * @param fluidDynamics
 * @param coordFn
 *
 * @return
 */
template<typename CoordFn>
DynamicArray<Range<Lattice::SizeType>> detectBlocks(const Lattice& lattice,
    Range<Lattice::SizeType> range, ViewPtr<Dynamics> fluidDynamics, CoordFn coordFn) noexcept
{
    DynamicArray<Range<Lattice::SizeType>> blocks;

    // Foreach boundary edge
    for (Lattice::SizeType i = range.first(); i < range.last(); ++i)
    {
        const auto& node1 = lattice[coordFn(i)];

        if (node1.getDynamics() != fluidDynamics)
            continue;

        const Lattice::SizeType start = i++;

        // Inner block
        for (; i < range.last(); ++i)
        {
            const auto& node2 = lattice[coordFn(i)];

            if (node2.getDynamics() != fluidDynamics)
                break;
        }

        // Insert block
        blocks.push_back({start, i});
    }

    return blocks;
}

/* ************************************************************************ */

/**
 * @brief Apply blocks to lattice.
 *
 * @param lattice
 * @param blocks
 * @param fluidDynamics
 * @param coordFn
 * @param applyFn
 *
 * @return
 */
template<typename CoordFn, typename ApplyFn>
void applyBlocks(Lattice& lattice, const DynamicArray<Range<Lattice::SizeType>>& blocks,
    ViewPtr<Dynamics> dynamics, ViewPtr<Dynamics> fluidDynamics, CoordFn coordFn,
    ApplyFn applyFn) noexcept
{
    // Foreach blocks
    for (const auto& block : blocks)
    {
        for (auto i = block.first(); i < block.last(); ++i)
        {
            const auto c = coordFn(i);
            auto& node = lattice[c];

            // Set boundary dynamics
            if (node.getDynamics() == fluidDynamics)
                node.setDynamics(dynamics);

            if (node.getDynamics() == dynamics)
                applyFn(node, c, block.getSize());
        }
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
    setName(config.get("name", getName()));
    setType(config.get("type", getType()));
    setPosition(config.get("position", getPosition()));
    setOffset(config.get("offset", getOffset()));
    setSize(config.get("size", getSize()));
    setInletProfileType(config.get("inlet-profile", getInletProfileType()));
    setInletVelocity(config.get("inlet-velocity", getInletVelocity()));
    setInletFlow(config.get("inlet-flow-rate", getInletFlow()));
}

/* ************************************************************************ */

void Boundary::storeConfig(config::Configuration& config) const
{
    config.set("name", getName());
    config.set("type", getType());
    config.set("position", getPosition());
    config.get("offset", getOffset());
    config.get("size", getSize());
    config.set("inlet-profile", getInletProfileType());
    config.set("inlet-velocity", getInletVelocity());
    config.set("inlet-flow-rate", getInletFlow());
}

/* ************************************************************************ */

void Boundary::updateBlocks(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics)
{
    // No boundary
    if (getType() == Type::None)
        return;

    const auto gridSize = lattice.getSize();
    const auto offset = converter.convertLength(m_offset);
    const auto size = converter.convertLength(m_size);

    switch (getPosition())
    {
    case Position::Top:
    {
        // Boundary block
        const auto block = getBlock(offset, size, gridSize.getWidth());

        // Top edge
        m_blocks = detectBlocks(lattice, block, fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{i, gridSize.getHeight() - 1};
        });

        break;
    }

    case Position::Bottom:
    {
        // Boundary block
        const auto block = getBlock(offset, size, gridSize.getWidth());

        // Bottom edge
        m_blocks = detectBlocks(lattice, block, fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{i, 0};
        });

        break;
    }

    case Position::Left:
    {
        // Boundary block
        const auto block = getBlock(offset, size, gridSize.getHeight());

        // Left edge
        m_blocks = detectBlocks(lattice, block, fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{0, i};
        });

        break;
    }

    case Position::Right:
    {
        // Boundary block
        const auto block = getBlock(offset, size, gridSize.getHeight());

        // Right edge
        m_blocks = detectBlocks(lattice, block, fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{gridSize.getWidth() - 1, i};
        });

        break;
    }
    }
}

/* ************************************************************************ */

void Boundary::apply(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics)
{
    // No boundary
    if (getType() == Type::None)
        return;

    const auto gridSize = lattice.getSize();

    // Apply function
    auto applyFn = [&] (Node& node, Lattice::CoordinateType c, Lattice::SizeType width) {
        if (getType() == Type::Outlet)
        {
            node.defineDensity(1.0);
        }
        else if (getType() == Type::Inlet)
        {
            node.defineVelocity(converter.convertVelocity(
                inletVelocity(converter, c, width)
            ));
        }
    };

    switch (getPosition())
    {
    case Position::Top:
    {
        applyBlocks(lattice, m_blocks, getDynamics(), fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{i, gridSize.getHeight() - 1};
        }, applyFn);
        break;
    }

    case Position::Bottom:
    {
        applyBlocks(lattice, m_blocks, getDynamics(), fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{i, 0};
        }, applyFn);
        break;
    }

    case Position::Left:
    {
        applyBlocks(lattice, m_blocks, getDynamics(), fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{0, i};
        }, applyFn);
        break;
    }

    case Position::Right:
    {
        applyBlocks(lattice, m_blocks, getDynamics(), fluidDynamics, [&](Lattice::SizeType i) {
            return Lattice::CoordinateType{gridSize.getWidth() - 1, i};
        }, applyFn);
        break;
    }
    }
}

/* ************************************************************************ */

units::VelocityVector Boundary::inletVelocity(Converter& converter,
    Lattice::CoordinateType coord, Lattice::SizeType width) const noexcept
{
    auto type = getInletProfileType();

    if (type == InletProfileType::Auto)
        type = InletProfileType::Constant;

    if (type == InletProfileType::Constant)
    {
        // Calculate velocity
        const units::Velocity velocity = getInletFlow() != Zero
            ? converter.calculateVelocity(
                getInletFlow(),
                converter.convertLength(static_cast<RealType>(width)))
            : getInletVelocity()
        ;

        switch (m_position)
        {
        case Position::Top:
            return {Zero, -velocity};

        case Position::Bottom:
            return {Zero, velocity};

        case Position::Left:
            return {velocity, Zero};

        case Position::Right:
            return {-velocity, Zero};

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
