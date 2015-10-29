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

    executeReactions(step, Context(diffusion, cell, coords));
}

void Reactions::executeReactions(units::Time step, const Context& pointers)
{
    if (m_propensities.empty())
        initializePropensities(pointers);
    else
        refreshEnvPropensities(pointers);

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
            initializePropensities(pointers);
            return;
        }

        std::discrete_distribution<> distr(m_propensities.begin(), m_propensities.end());

        // time of reaction
        const auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
        time -= delta_time;

        executeRules(distr(gen), pointers);
    }
}

/* ************************************************************************ */

void Reactions::executeRules(unsigned int index, const Context& pointers)
{
    const auto& reaction = m_reactions[index];

    for (unsigned int moleculeIndex = 0; moleculeIndex < m_moleculeNames.size(); ++moleculeIndex)
    {
        const auto& moleculeName = m_moleculeNames[moleculeIndex];

        const auto change = reaction.getProduct(moleculeIndex) - reaction.getRequirement(moleculeIndex);
        const auto env_change = reaction.getEnvProduct(moleculeIndex) - reaction.getEnvRequirement(moleculeIndex);

        if (!change && !env_change)
            return;

        if (change)
            pointers.cell.changeMoleculeCount(moleculeName, change);

        if (env_change)
            changeMoleculesInEnvironment(env_change, moleculeName, pointers);

        refreshPropensities(moleculeIndex, pointers);
    }
}

/* ************************************************************************ */

void Reactions::changeMoleculesInEnvironment(const int change, const String& name, const Context& pointers)
{
    assert(change != 0);

    // Get signal ID
    const auto id = pointers.diffusion->requireSignalId(name);

    // Change amount of molecules
    changeMolecules(pointers.cell.getSimulation(), *pointers.diffusion, pointers.coords, id, change);
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
        m_reactions.resize(m_moleculeNames.size());

    return m_moleculeNames.size() - 1;
}

/* ************************************************************************ */

PropensityType Reactions::computePropensity(const unsigned int index, const Context& context)
{
    // Evaulate condition
    if (!m_reactions[index].evaluateCondition(context))
        return 0;

    PropensityType local = m_reactions[index].evaluateRate(context);

    for (unsigned int i = 0; i < m_moleculeNames.size(); i++)
    {
        // Intracellular
        const auto number = context.cell.getMoleculeCount(m_moleculeNames[i]);
        if (m_reactions[index].getRequirement(i) != 0u)
            local *= number;

        // Intercellular
        const auto id = context.diffusion->getSignalId(m_moleculeNames[i]);
        if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
        {
            const auto numberEnv = getMolarConcentration(*context.diffusion, context.coords, id).value();
            if (m_reactions[index].getEnvRequirement(i) != 0u)
                local *= numberEnv;
        }
    }

    return local;
}

/* ************************************************************************ */

void Reactions::initializePropensities(const Context& pointers)
{
    m_propensities.clear();
    for (unsigned int i = 0; i < m_reactions.size(); i++)
    {
        m_propensities.push_back(computePropensity(i, pointers));
    }
}

/* ************************************************************************ */

void Reactions::refreshPropensities(const unsigned int index, const Context& pointers)
{
    for (unsigned int i = 0; i < m_reactions.size(); i++)
    {
        if (m_reactions[i].getRequirement(index) || m_reactions[i].getEnvRequirement(index))
        {
            m_propensities[i] = computePropensity(i, pointers);
        }
    }
}

/* ************************************************************************ */

void Reactions::refreshEnvPropensities(const Context& pointers)
{
    for (unsigned int i = 0; i < m_reactions.size(); i++)
    {
        for (unsigned int j = 0; j < m_moleculeNames.size(); j++)
        {
            if (m_reactions[i].getEnvRequirement(j))
            {
                m_propensities[i] = computePropensity(i, pointers);
            }
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
