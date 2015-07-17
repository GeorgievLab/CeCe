/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* ************************************************************************ */

#include "IntercellularReactions.hpp"

// C++
#include <cassert>

// Plugin
#include "Diffusion.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

Reactions<ReqProd>::NumberType IntercellularReactions::computePropensity(const unsigned int index, const plugin::cell::CellBase& cell, simulator::Simulation& simulation)
{
    NumberType local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        if (m_rules[index][i].mustnt_have)
            return 0;
        if (m_rules[index][i].requirement != 0u)
        {
            unsigned int number = cell.getMoleculeCount(m_ids[i]);
            if (m_rules[index][i].requirement > number)
                return 0;
            local *= number;
        }
        if (m_rules[index][i].env_requirement != 0u)
        {
            // TODO: change to requireModule after it returns the module
            auto diffusion = simulation.useModule<plugin::diffusion::Module>("diffusion");
            assert(diffusion);

            // Get coordinates
            const auto& worldSize = simulation.getWorldSize();
            const auto coords = getCoordinates(diffusion->getGridSize(), worldSize, *cell);

            // Get signal ID
            auto id = diffusion->getSignalId(m_ids[i]);

            if (id == plugin::diffusion::Module::INVALID_SIGNAL_ID)
                throw InvalidArgumentException("Unknown signal molecule '" + m_ids[i] + "' in diffusion");

            unsigned int number = getAmountOfMolecules(*diffusion, coords, id);
            if (m_rules[index][i].requirement > number)
                return 0;
            local *= number;
        }
    }
    return local;
}

/* ************************************************************************ */

void IntercellularReactions::initializePropensities(const plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        propensities.push_back(computePropensity(i, cell));
    }
}

/* ************************************************************************ */

void IntercellularReactions::refreshPropensities(unsigned int index, const plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_rules[i][index].requirement != 0)
        {
            propensities[i] = computePropensity(i, cell);
        }
    }
}

/* ************************************************************************ */

void IntercellularReactions::executeReaction(const unsigned int index, plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change != 0)
        {
            cell.changeMoleculeCount(m_ids[i], change);
            refreshPropensities(i, cell);
            return;
        }
        change = m_rules[index][i].env_product - m_rules[index][i].env_requirement;
        if (change != 0)
        {
            changeMoleculesInEnvironment(m_ids[(i], change);
            refreshPropensities(i, cell);
            return;
        }
    }
}

/* ************************************************************************ */

void IntercellularReactions::operator()(simulator::Object& object, simulator::Simulation&, units::Duration step)
{
    auto& cell = getCell(object);

    if (propensities.empty())
    {
        initializePropensities(cell);
    }

    executeReactions(step, [this, &cell](unsigned int index)
    {
        executeReaction(index, cell);
    });
}

/* ************************************************************************ */

void IntercellularReactions::changeMoleculesInEnvironment(const String& name, const int change)
{
    assert(change != 0);

    // TODO: change to requireModule after it returns the module
    auto diffusion = m_simulation->useModule<plugin::diffusion::Module>("diffusion");
    assert(diffusion);

    // Get signal ID
    auto id = diffusion->getSignalId(name);

    if (id == plugin::diffusion::Module::INVALID_SIGNAL_ID)
        throw InvalidArgumentException("Unknown signal molecule '" + name + "' in diffusion");

    // Get coordinates
    const auto& worldSize = m_simulation->getWorldSize();
    const auto coords = getCoordinates(diffusion->getGridSize(), worldSize, *cell);

    // Change amount of molecules
    // FIXME: #36
    changeMolecules(*diffusion, coords, id, 100 * change);
}

/* ************************************************************************ */

void IntercellularReactions::extendAbsorption(const DynamicArray<String>& ids_plus, const float rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        unsigned int index = getIndexOfMoleculeColumn(ids_plus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{0,1});
            array.push_back(ReqProd{1,0});
            continue;
        }
        array[index].product += 1;
        array[index + 1].requirement += 1;
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

void IntercellularReactions::extendExpression(const DynamicArray<String>& ids_minus, const float rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        unsigned int index = getIndexOfMoleculeColumn(ids_minus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{1,0});
            array.push_back(ReqProd{0,1});
            continue;
        }
        array[index].requirement += 1;
        array[index + 1].product += 1;
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

void IntercellularReactions::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const float rate)
{
    if (!ids_minus.empty() && ids_minus[0] == "env")
    {
        if (ids_minus.size() == 1)
            extendAbsorption(ids_plus, rate);
        else
            Log::warning("This reaction is not valid. ENV tag must be alone.");
        return;
    }

    if (!ids_plus.empty() && ids_plus[0] == "env")
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

}
}

/* ************************************************************************ */
