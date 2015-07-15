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

#include "ReactionIntercellular.hpp"

// C++
#include <cassert>

// Plugin
#include "Diffusion.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

float ReactionIntercellular::computePropensity(const unsigned int index)
{
    float local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        if (m_rules[index][i].requirement == 0)
            continue;
        if (i % 2 == 0)
        {
            unsigned int number = cell->getMoleculeCount(m_ids[i / 2]);
            if (m_rules[index][i].requirement > number)
                return 0;
            local *= number;
        }
        else
        {
            // TODO: change to requireModule after it returns the module
            auto diffusion = m_simulation->useModule<plugin::diffusion::Module>("diffusion");
            assert(diffusion);

            // Get coordinates
            const auto& worldSize = m_simulation->getWorldSize();
            const auto coords = getCoordinates(diffusion->getGridSize(), worldSize, *cell);

            // Get signal ID
            auto id = diffusion->getSignalId(m_ids[i / 2]);

            if (id == plugin::diffusion::Module::INVALID_SIGNAL_ID)
                throw InvalidArgumentException("Unknown signal molecule '" + m_ids[i / 2] + "' in diffusion");

            unsigned int number = getAmountOfMolecules(*diffusion, coords, id);
            if (m_rules[index][i].requirement > number)
                return 0;
            local *= number;
        }
    }
    return local;
}

/* ************************************************************************ */

void ReactionIntercellular::executeReaction(const unsigned int index)
{
    for (unsigned int i = 0; i < m_ids.size()*2; i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change == 0)
            continue;
        if (i % 2 == 0)
            cell->changeMoleculeCount(m_ids[i], change);
        else
            changeMoleculesInEnvironment(m_ids[(i - 1) / 2], change);
        refreshPropensities(i);
    }
}

/* ************************************************************************ */

void ReactionIntercellular::changeMoleculesInEnvironment(const String& name, const int change)
{
    /* todo: přidat počet molekul typu m_ids[i-1] na pozici kolem bunky (random device? - will be added later)*/
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

void ReactionIntercellular::extendAbsorption(const DynamicArray<String>& ids_plus, const float rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_plus[i]);
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

void ReactionIntercellular::extendExpression(const DynamicArray<String>& ids_minus, const float rate)
{
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_minus[i]);
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

void ReactionIntercellular::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const float rate)
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

    DynamicArray<ReqProd> array;

    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());

    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_minus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{1,0});
            array.push_back(ReqProd{0, 0});
            continue;
        }
        array[index].requirement += 1;
    }

    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_plus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{0,1});
            array.push_back(ReqProd{0, 0});
            continue;
        }
        array[index].product += 1;
    }

    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

int ReactionIntercellular::getIndexOf(const String& id)
{
    auto pointer = std::find(m_ids.begin(), m_ids.end(), id);
    if (pointer == m_ids.end())
    {
        for (unsigned int i = 0; i < m_rules.size(); i++)
        {
            m_rules[i].emplace_back();
            m_rules[i].emplace_back();
        }
        m_ids.push_back(id);
        return m_ids.size() - 1;
    }
    return std::distance(m_ids.begin(), pointer);
}

/* ************************************************************************ */

bool ReactionIntercellular::areEqualRules(const Reaction& rhs, unsigned int index1, unsigned int index2)
{
    return true;
    // TODO !!!
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
