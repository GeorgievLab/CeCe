/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

#include "Reactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

void Reactions::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step)
{
    auto& cell = object.castThrow<cell::CellBase>();
    auto diffusion = simulation.useModule<plugin::diffusion::Module>("diffusion");
    const auto& worldSize = simulation.getWorldSize();
    const auto& coords = getCoordinates(diffusion->getGridSize(), worldSize, cell);

    executeReactions(step, diffusion, cell, coords);
}

void Reactions::executeReactions(
    units::Time step,
    plugin::diffusion::Module* diffusion,
    plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    if (m_propensities.empty())
        initializePropensities(diffusion, cell, coords);
    else
        refreshEnvPropensities(diffusion, cell, coords);

    // initialize time
    units::Time time = Zero;

    // initialize random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0, 1);

    // Gillespie algorithm + tau-leaping + dependency graph(hidden inside execute reaction)
    while (time < step)
    {
        PropensityType sum = std::accumulate(m_propensities.begin(), m_propensities.end(), 0.0f);

        if (sum == 0)
        {
            initializePropensities(diffusion, cell, coords);
            return;
        }

        std::discrete_distribution<> distr(m_propensities.begin(), m_propensities.end());

        // time of reaction
        const auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
        time -= delta_time;

        executeRules(distr(gen), diffusion, cell, coords);
    }
}

/* ************************************************************************ */

void Reactions::executeRules(
    unsigned int index,
    plugin::diffusion::Module* diffusion,
    plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    const auto& reaction = m_reactions[index];

    for (unsigned int moleculeIndex = 0; moleculeIndex < m_moleculeNames.size(); ++moleculeIndex)
    {
        const auto& moleculeName = m_moleculeNames[moleculeIndex];

        const auto change = reaction.rules[moleculeIndex].product - reaction.rules[moleculeIndex].requirement;
        const auto env_change = reaction.rules[moleculeIndex].env_product - reaction.rules[moleculeIndex].env_requirement;

        if (!change && !env_change)
            return;

        if (change)
            cell.changeMoleculeCount(moleculeName, change);

        if (env_change)
            changeMoleculesInEnvironment(env_change, moleculeName, diffusion, cell.getSimulation(), coords);

        refreshPropensities(moleculeIndex, diffusion, cell, coords);
    }
}

/* ************************************************************************ */

void Reactions::changeMoleculesInEnvironment(
    const int change,
    const String& name,
    plugin::diffusion::Module* diffusion,
    const simulator::Simulation& simulation,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    assert(change != 0);

    // Get signal ID
    const auto id = diffusion->requireSignalId(name);

    // Change amount of molecules
    changeMolecules(simulation, *diffusion, coords, id, change);
}

/* ************************************************************************ */

unsigned int Reactions::getMoleculeIndex(const String& name)
{
    auto pointer = std::find(m_moleculeNames.begin(), m_moleculeNames.end(), name);

    // Molecule exists in reactions
    if (pointer != m_moleculeNames.end())
        return std::distance(m_moleculeNames.begin(), pointer);

    // Store molecule
    m_moleculeNames.push_back(name);

    // Resize all reactions
    for (auto& reaction : m_reactions)
        reaction.rules.resize(m_moleculeNames.size());

    return m_moleculeNames.size() - 1;
}

/* ************************************************************************ */

Reactions::PropensityType Reactions::computePropensity(
    const unsigned int index,
    plugin::diffusion::Module* diffusion,
    const plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    PropensityType local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        const auto number = cell.getMoleculeCount(m_moleculeNames[i]);

        bool condEnv = true;

        // Get signal ID
        const auto id = diffusion->getSignalId(m_moleculeNames[i]);
        if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
        {
            const auto numberEnv = getMolarConcentration(*diffusion, coords, id).value();

            condEnv =
                // >
                (m_rules[index][i].env_less && (numberEnv <= m_rules[index][i].env_requirement)) ||
                // <=
                (!m_rules[index][i].env_less && (numberEnv >= m_rules[index][i].env_requirement))
            ;
        }

        if (!cond || !condEnv)
            return 0;

        if (m_rules[index][i].requirement != 0u)
            local *= number;
    }
    return local;
}

/* ************************************************************************ */

void Reactions::initializePropensities(
    plugin::diffusion::Module* diffusion,
    const plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    m_propensities.clear();
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        m_propensities.push_back(computePropensity(i, cell, diffusion, coords));
    }
}

/* ************************************************************************ */

void Reactions::refreshPropensities(
    const unsigned int index,
    plugin::diffusion::Module* diffusion,
    const plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_reactions[i].rules[index].requirement)
        {
            m_propensities[i] = computePropensity(i, cell, diffusion, coords);
        }
    }
}

/* ************************************************************************ */

void Reactions::refreshEnvPropensities(
    plugin::diffusion::Module* diffusion,
    const plugin::cell::CellBase& cell,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_reactions[i].rules[index].env_requirement)
        {
            m_propensities[i] = computePropensity(i, cell, diffusion, coords);
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
