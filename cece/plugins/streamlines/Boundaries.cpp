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
#include "cece/config/Configuration.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse array of values from string.
 *
 * @param str Source string.
 *
 * @return
 */
template<typename T, std::size_t N>
StaticArray<T, N> split(String str)
{
    StaticArray<T, N> array;
    InStringStream is(std::move(str));

    // Read values
    for (std::size_t i = 0; i < N; ++i)
        is >> std::skipws >> array[i];

    return array;
}

/* ************************************************************************ */

}

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

bool Boundaries::isBoundaryDynamics(ViewPtr<Dynamics> dynamics) const noexcept
{
    for (const auto& boundary : m_boundaries)
    {
        if (boundary.getDynamics() == dynamics)
            return true;
    }

    return false;
}

/* ************************************************************************ */

bool Boundaries::isBoundaryDynamics(ViewPtr<Dynamics> dynamics, Boundary::Type type) const noexcept
{
    for (const auto& boundary : m_boundaries)
    {
        if ((boundary.getDynamics() == dynamics) && (boundary.getType() == type))
            return true;
    }

    return false;
}

/* ************************************************************************ */

void Boundaries::initDefault()
{
    if (!m_boundaries.empty())
        return;

    m_boundaries.resize(4);

    for (int i = 0; i < 4; ++i)
        m_boundaries[i].setPosition(static_cast<Boundary::Position>(i));
}

/* ************************************************************************ */

void Boundaries::init(Lattice& lattice, ViewPtr<Dynamics> fluidDynamics)
{
    // Nothing to do
}

/* ************************************************************************ */

void Boundaries::updateBlocks(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics)
{
    // Apply boundary conditions
    for (auto& boundary : m_boundaries)
        boundary.updateBlocks(lattice, converter, fluidDynamics);
}

/* ************************************************************************ */

void Boundaries::applyConditions(Lattice& lattice, Converter& converter, ViewPtr<Dynamics> fluidDynamics)
{
    // Apply boundary conditions
    for (auto& boundary : m_boundaries)
        boundary.apply(lattice, converter, fluidDynamics);
}

/* ************************************************************************ */

void Boundaries::loadConfig(const config::Configuration& config)
{
    // Layout
    if (config.has("layout"))
    {
        // Create default
        initDefault();

        const auto layout = split<String, 4>(config.get("layout"));

        for (std::size_t i = 0; i < layout.size(); ++i)
        {
            if (layout[i] == "inlet")
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
        initDefault();

        const auto velocity = config.get<units::Velocity>("inlet-velocity");

        for (auto& boundary : m_boundaries)
            boundary.setInletVelocity(velocity);
    }
    else if (config.has("inlet-velocities"))
    {
        initDefault();

        const auto velocities = split<units::Velocity, 4>(config.get("inlet-velocities"));

        for (std::size_t i = 0; i < velocities.size(); ++i)
            m_boundaries[i].setInletVelocity(velocities[i]);
    }

    if (config.has("inlet-type"))
    {
        initDefault();

        const auto typeStr = config.get<String>("inlet-type");
        Boundary::InletProfileType type = Boundary::InletProfileType::Auto;

        if (typeStr == "constant")
            type = Boundary::InletProfileType::Constant;

        for (auto& boundary : m_boundaries)
            boundary.setInletProfileType(type);
    }
    else if (config.has("inlet-types"))
    {
        initDefault();

        const auto types = split<String, 4>(config.get("inlet-types"));

        for (std::size_t i = 0; i < types.size(); ++i)
        {
            Boundary::InletProfileType type = Boundary::InletProfileType::Auto;

            if (types[i] == "constant")
                type = Boundary::InletProfileType::Constant;

            m_boundaries[i].setInletProfileType(type);
        }
    }

    // Inlets
    for (auto&& boundaryConfig : config.getConfigurations("boundary"))
    {
        Boundary boundary;
        boundary.loadConfig(boundaryConfig);
        m_boundaries.push_back(std::move(boundary));
    }
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
