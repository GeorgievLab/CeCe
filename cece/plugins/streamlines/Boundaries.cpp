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
#include "cece/plugins/streamlines/Module.hpp"

// C++
#include <cstddef>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Zero.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Read an array of T.
 *
 * @param is    Input stream.
 * @param array Output array.
 *
 * @return is.
 */
template<typename T, std::size_t N>
InStream& operator>>(InStream& is, StaticArray<T, N>& array)
{
    for (std::size_t i = 0; i < N; ++i)
        is >> std::skipws >> array[i];

    return is;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

Boundaries::Boundaries(simulator::Simulation& simulation)
    : m_simulation(simulation)
{
    // Initialize boundary positions
    for (std::size_t i = 0; i < m_boundaries.size(); ++i)
        m_boundaries[i].setPosition(static_cast<Boundary::Position>(i));
}

/* ************************************************************************ */

units::Velocity Boundaries::getMaxInletVelocity() const noexcept
{
    units::Velocity velocity = Zero;

    for (const auto& boundary : m_boundaries)
    {
        // Only inlets
        if (boundary.getType() != Boundary::Type::Inlet)
            continue;

        if (velocity < boundary.getInletVelocity())
            velocity = boundary.getInletVelocity();
    }

    return velocity;
}

/* ************************************************************************ */

void Boundaries::init()
{
    // Create barriers
    for (auto& boundary : m_boundaries)
    {
        if (boundary.getBarrierObject())
        {
            // Barrier recreation is not needed
            if (boundary.getType() == Boundary::Type::Barrier)
                continue;

            getSimulation().deleteObject(boundary.getBarrierObject());
            boundary.setBarrierObject(nullptr);
        }
        else if (boundary.getType() == Boundary::Type::Barrier)
        {
            boundary.initBarrier(getSimulation());
        }
    }
}

/* ************************************************************************ */

void Boundaries::applyConditions(Converter& converter, Lattice& lattice, ViewPtr<Dynamics> fluidDynamics)
{
    // Init boundaries
    for (auto& boundary : m_boundaries)
    {
        boundary.findHoles(lattice, fluidDynamics);
        boundary.initInletOutlet(converter, lattice, fluidDynamics);
    }
}

/* ************************************************************************ */

void Boundaries::loadConfig(const config::Configuration& config)
{
    // Layout
    if (config.has("layout"))
    {
        const auto layout = config.get<StaticArray<String, 4>>("layout");

        for (std::size_t i = 0; i < layout.size(); ++i)
        {
            if (layout[i] == "barrier")
                m_boundaries[i].setType(Boundary::Type::Barrier);
            else if (layout[i] == "inlet")
                m_boundaries[i].setType(Boundary::Type::Inlet);
            else if (layout[i] == "outlet")
                m_boundaries[i].setType(Boundary::Type::Outlet);
            else
                m_boundaries[i].setType(Boundary::Type::None);
        }
    }

    // Inlet velocities
    if (config.has("inlet-velocity"))
    {
        const auto velocity = config.get<units::Velocity>("inlet-velocity");

        for (auto& boundary : m_boundaries)
            boundary.setInletVelocity(velocity);
    }
    else if (config.has("inlet-velocities"))
    {
        const auto velocities = config.get<StaticArray<units::Velocity, 4>>("inlet-velocities");

        for (std::size_t i = 0; i < velocities.size(); ++i)
            m_boundaries[i].setInletVelocity(velocities[i]);
    }

    if (config.has("inlet-type"))
    {
        const auto typeStr = config.get<String>("inlet-type");
        Boundary::InletProfileType type = Boundary::InletProfileType::Auto;

        if (typeStr == "constant")
            type = Boundary::InletProfileType::Constant;
        else if (typeStr == "parabolic")
            type = Boundary::InletProfileType::Parabolic;

        for (auto& boundary : m_boundaries)
            boundary.setInletProfileType(type);
    }
    else if (config.has("inlet-types"))
    {
        const auto types = config.get<StaticArray<String, 4>>("inlet-types");

        for (std::size_t i = 0; i < types.size(); ++i)
        {
            Boundary::InletProfileType type = Boundary::InletProfileType::Auto;

            if (types[i] == "constant")
                type = Boundary::InletProfileType::Constant;
            else if (types[i] == "parabolic")
                type = Boundary::InletProfileType::Parabolic;

            m_boundaries[i].setInletProfileType(type);
        }
    }

    // TODO: separate elements
}

/* ************************************************************************ */

void Boundaries::storeConfig(config::Configuration& config) const
{

}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
