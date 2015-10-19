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

// C++
#include <cassert>

// Plugin
#include "Diffusion.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

IntercellularReactions::PropensityType IntercellularReactions::computePropensity(
    const unsigned int index,
    const plugin::cell::CellBase& cell,
    plugin::diffusion::Module* diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    PropensityType local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        const auto number = cell.getMoleculeCount(m_moleculeNames[i]);

        // TODO: check if it's OK
        const bool cond =
            // >
            (m_rules[index][i].less && (number <= m_rules[index][i].requirement)) ||
            // <=
            (!m_rules[index][i].less && (number >= m_rules[index][i].requirement))
        ;

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

void IntercellularReactions::executeReaction(
    const unsigned int index,
    plugin::cell::CellBase& cell,
    plugin::diffusion::Module* diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    // Alias for reaction row
    const auto& reaction = m_rules[index];

    // Foreach molecules in reaction
    for (unsigned int moleculeId = 0; moleculeId < m_moleculeNames.size(); ++moleculeId)
    {
        const auto& moleculeName = m_moleculeNames[moleculeId];

        // Intracellular reaction
        const auto change = reaction[moleculeId].product - reaction[moleculeId].requirement;
        if (change != 0)
        {
            cell.changeMoleculeCount(moleculeName, change);
            refreshPropensities(moleculeId, cell, diffusion, coords);
        }

        // Intercellular reaction
        // No need to substract env_requirement because the reaction is executed only when requirement
        // is satisfied.
        if (reaction[moleculeId].env_product)
        {
            changeMoleculesInEnvironment(cell.getSimulation(), moleculeName, reaction[moleculeId].env_product, diffusion, coords);
            initializePropensities(cell, diffusion, coords);
        }
    }
}

/* ************************************************************************ */

void IntercellularReactions::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step)
{
    auto& cell = object.castThrow<cell::CellBase>();
    auto diffusion = simulation.useModule<plugin::diffusion::Module>("diffusion");
    const auto& worldSize = simulation.getWorldSize();
    const auto& coords = getCoordinates(diffusion->getGridSize(), worldSize, cell);

    executeReactions(step, [this, &cell, &diffusion, &coords](unsigned int index) {
        executeReaction(index, cell, diffusion, coords);
    }, [this, &cell, &diffusion, &coords] () {
        initializePropensities(cell, diffusion, coords);
    });
}

/* ************************************************************************ */

void IntercellularReactions::changeMoleculesInEnvironment(
    const simulator::Simulation& simulation,
    const String& name,
    const int change,
    plugin::diffusion::Module* diffusion,
    const DynamicArray<plugin::diffusion::Module::Coordinate>& coords)
{
    assert(change != 0);

    // Get signal ID
    const auto id = diffusion->requireSignalId(name);

    // Change amount of molecules
    changeMolecules(simulation, *diffusion, coords, id, change);
}

/* ************************************************************************ */

void IntercellularReactions::extendAbsorption(const DynamicArray<String>& ids_plus, const RateType rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        if (ids_plus[i] == "null")
        {
            Log::warning("NULL tag doesnt make sense here.");
            return;
        }
        unsigned int index = getMoleculeId(ids_plus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{0, 1, 1, 0});
            continue;
        }
        array[index].env_requirement += 1;
        array[index].product += 1;
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

void IntercellularReactions::extendExpression(const DynamicArray<String>& ids_minus, const RateType rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        if (ids_minus[i] == "null")
        {
            Log::warning("NULL tag doesnt make sense here.");
            return;
        }
        unsigned int index = getMoleculeId(ids_minus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{1, 0, 0, 1});
            continue;
        }
        array[index].env_product += 1;
        array[index].requirement += 1;
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

void IntercellularReactions::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
{
    if (std::find(ids_minus.begin(), ids_minus.end(), "env") != ids_minus.end())
    {
        if (ids_minus.size() == 1)
            extendAbsorption(ids_plus, rate);
        else
            Log::warning("This reaction is not valid. ENV tag must be alone.");
        return;
    }
    if (std::find(ids_plus.begin(), ids_plus.end(), "env") != ids_plus.end())
    {
        if (ids_plus.size() == 1)
            extendExpression(ids_minus, rate);
        else
            Log::warning("This reaction is not valid. ENV tag must be alone.");
        return;
    }
    extendIntracellular(ids_plus, ids_minus, rate);
}

/* ************************************************************************ */

void IntercellularReactions::extendIntracellular(
    const DynamicArray<String>& ids_plus,
    const DynamicArray<String>& ids_minus,
    const RateType rate
)
{
    // initialize array
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());

    // extend with requirements
    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        if (ids_minus[i] == "null" || ids_minus[i] == "env_null")
            continue;

        // Produce to environment directly
        const bool fromEnv = (ids_plus[i].substr(0, 4) == "env_");

        // Get real name
        const String nameReal = fromEnv ? ids_plus[i].substr(4) : ids_plus[i];

        unsigned int index = getMoleculeId(ids_minus[i]);
        if (index == array.size())
        {
            if (fromEnv)
                array.push_back(ReqProd{0, 0, 1, 0});
            else
                array.push_back(ReqProd{1, 0});
        }
        else
        {
            if (fromEnv)
                array[index].env_requirement += 1;
            else
                array[index].requirement += 1;
        }
    }

    //extend with products
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        if (ids_plus[i] == "null" || ids_plus[i] == "env_null")
            continue;

        // Produce to environment directly
        const bool toEnv = (ids_plus[i].substr(0, 4) == "env_");

        // Get real name
        const String nameReal = toEnv ? ids_plus[i].substr(4) : ids_plus[i];

        unsigned int index = getMoleculeId(nameReal);
        if (index == array.size())
        {
            if (toEnv)
                array.push_back(ReqProd{0, 0, 0, 1});
            else
                array.push_back(ReqProd{0, 1});
        }
        else
        {
            if (toEnv)
                array[index].env_product += 1;
            else
                array[index].product += 1;
        }
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

void IntercellularReactions::addCondition(const Condition& condition, DynamicArray<ReqProd>& noCond)
{
    // Condition is based on environmental molecule
    const bool fromEnv = (condition.name.substr(0, 4) == "env_");

    // Get real name
    const String nameReal = fromEnv ? condition.name.substr(4) : condition.name;

    // add unmodified reaction rule after OR is reached
    if (condition.clone)
    {
        // backup of unmodified reaction can be outdated (shorter)
        noCond.resize(m_moleculeNames.size());

        m_rules.push_back(noCond);
        m_rates.push_back(m_rates[m_rates.size() - 1]);
    }

    // get index of required molecule
    const auto moleculeId = getMoleculeId(nameReal);

    // Alias to last reaction
    auto& reaction = m_rules[m_rules.size() - 1];

    // add requirement
    if (fromEnv)
    {
        const auto diff = std::max<RealType>(condition.requirement - reaction[moleculeId].env_requirement, 0);

        // add requirement
        reaction[moleculeId].env_less = condition.less;
        reaction[moleculeId].env_requirement += diff;
        reaction[moleculeId].env_product += diff;
    }
    else
    {
        const auto diff = std::max<RealType>(condition.requirement - reaction[moleculeId].requirement, 0);

        // add requirement
        reaction[moleculeId].less = condition.less;
        reaction[moleculeId].requirement += diff;
        reaction[moleculeId].product += diff;
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
