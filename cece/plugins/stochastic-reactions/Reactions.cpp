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

// Declaration
#include "Reactions.hpp"

// Reactions
#include "ReactionsParser.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/// Random device
std::random_device g_rd;

/* ************************************************************************ */

UniquePtr<program::Program> Reactions::clone() const
{
    return makeUnique<Reactions>(*this);
}

/* ************************************************************************ */

void Reactions::loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
{
    *this = parseReactions(config.getContent());
}

/* ************************************************************************ */

void Reactions::call(simulator::Simulation& simulation, object::Object& object, units::Time dt)
{
    // get context
    auto& cell = object.castThrow<cell::CellBase>();
    const auto& worldSize = simulation.getWorldSize();
    const auto& parameters = simulation.getParameters();

    auto diffusion = simulation.getModule<plugin::diffusion::Module>("diffusion");
    DynamicArray<plugin::diffusion::Module::Coordinate> coords;
    if (diffusion != nullptr)
        coords = getCoordinates(diffusion->getGridSize(), worldSize, cell);

    // start
    executeReactions(dt, Context(diffusion, cell, &coords, parameters));
}

/* ************************************************************************ */

void Reactions::executeReactions(units::Time step, const Context& pointers)
{
    // initialize for iteration
    initializePropensities(pointers);
    auto timeleft = step;

    // initialize random device
    std::mt19937 gen(g_rd());
    std::uniform_real_distribution<> rand(0, 1);

    // Gillespie algorithm + tau-leaping
    while (true)
    {
        PropensityType sum = std::accumulate(m_propensities.begin(), m_propensities.end(), 0.0f);

        if (sum == 0)
        {
            // no reaction has happened
            return;
        }

        // initialize discrete random distribution
        std::discrete_distribution<> distr(m_propensities.begin(), m_propensities.end());

        // get time of reaction
        const auto delta_time = - (step / sum) * std::log(rand(gen));

        // quit if time exceeds iteration time
        if (timeleft < delta_time)
            break;

        // subtract reaction time from iteration time
        timeleft -= delta_time;

        // decide which reaction happened
        const auto reactionIndex = distr(gen);

        // execute
        executeRules(reactionIndex, pointers);
    }
}

/* ************************************************************************ */

void Reactions::executeRules(unsigned int index, const Context& pointers)
{
    Assert(index < m_reactions.size());

    const auto& reaction = m_reactions[index];

    for (unsigned int moleculeIndex = 0; moleculeIndex < m_moleculeNames.size(); ++moleculeIndex)
    {
        // get the actual change of current molecule when this reaction gets executed
        const auto change = reaction.getProduct(moleculeIndex) - reaction.getRequirement(moleculeIndex);
        const auto env_change = reaction.getEnvProduct(moleculeIndex) - reaction.getEnvRequirement(moleculeIndex);

        // exit if the reaction doesnt change anything
        if (!change && !env_change)
            continue;

        const auto& moleculeName = m_moleculeNames[moleculeIndex];

        // change molecule count inside the cell
        if (change)
            pointers.cell.changeMoleculeCount(moleculeName, change);

        // change molecule count in diffusion
        if (env_change)
            changeMoleculesInEnvironment(env_change, moleculeName, pointers);

        // refresh propensities
        refreshPropensities(pointers);
    }
}

/* ************************************************************************ */

void Reactions::changeMoleculesInEnvironment(const int change, const String& name, const Context& pointers)
{
    // get signal ID
    const auto id = pointers.diffusion->requireSignalId(name);

    // change amount of molecules
    changeMolecules(pointers.cell.getSimulation(), *pointers.diffusion, *pointers.coords, id, change);
}

/* ************************************************************************ */

unsigned int Reactions::getMoleculeIndex(const String& name)
{
    auto pointer = std::find(m_moleculeNames.begin(), m_moleculeNames.end(), name);

    // molecule exists in reactions
    if (pointer != m_moleculeNames.end())
        return std::distance(m_moleculeNames.begin(), pointer);

    // store molecule
    m_moleculeNames.push_back(name);

    // resize all reactions
    for (auto& reaction : m_reactions)
        reaction.extend();

    return m_moleculeNames.size() - 1;
}

/* ************************************************************************ */

PropensityType Reactions::computePropensity(const unsigned int index, const Context& context)
{
    // evaulate condition
    if (!m_reactions[index].evaluateCondition(context))
        return 0;

    PropensityType local = m_reactions[index].evaluateRate(context);

    for (unsigned int moleculeIndex = 0; moleculeIndex < m_moleculeNames.size(); moleculeIndex++)
    {
        // intracellular
        if (m_reactions[index].getRequirement(moleculeIndex) != 0u)
        {
            const auto number = context.cell.getMoleculeCount(m_moleculeNames[moleculeIndex]);
            local *= number;
        }

        // intercellular
        if (m_reactions[index].getEnvRequirement(moleculeIndex) != 0u)
        {
            const auto id = context.diffusion->getSignalId(m_moleculeNames[moleculeIndex]);
            if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
            {
                const auto numberEnv = getMolarConcentration(*context.diffusion, *context.coords, id).value();
                local *= numberEnv;
            }
        }
    }

    return local;
}

/* ************************************************************************ */

void Reactions::initializePropensities(const Context& pointers)
{
    // clear array first
    m_propensities.clear();

    // set length
    m_propensities.reserve(m_reactions.size());

    // fill
    for (unsigned int i = 0; i < m_reactions.size(); i++)
    {
        m_propensities.push_back(computePropensity(i, pointers));
    }
}

/* ************************************************************************ */

void Reactions::refreshPropensities(const Context& pointers)
{
    for (unsigned int i = 0; i < m_reactions.size(); i++)
    {
        m_propensities[i] = computePropensity(i, pointers);
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
