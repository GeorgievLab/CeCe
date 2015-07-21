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

#include "IntracellularReactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

IntracellularReactions::PropensityType IntracellularReactions::computePropensity(const unsigned int index, const plugin::cell::CellBase& cell)
{
    auto local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        if (m_rules[index][i].mustnt_have)
            return 0;
        if (m_rules[index][i].requirement == 0u)
            continue;
        if (m_rules[index][i].requirement > cell.getMoleculeCount(m_ids[i]))
            return 0;
        local *= cell.getMoleculeCount(m_ids[i]);
    }
    return local;
}

/* ************************************************************************ */

void IntracellularReactions::initializePropensities(const plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        propensities.push_back(computePropensity(i, cell));
    }
}

/* ************************************************************************ */

void IntracellularReactions::refreshPropensities(const unsigned int index, const plugin::cell::CellBase& cell)
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

void IntracellularReactions::executeReaction(const unsigned int index, plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change != 0)
        {
            cell.addMolecules(m_ids[i], change);
            refreshPropensities(i, cell);
        }
    }
}

/* ************************************************************************ */

void IntracellularReactions::operator()(simulator::Object& object, simulator::Simulation&, units::Duration step)
{
    auto& cell = getCell(object);

    executeReactions(step, [this, &cell](unsigned int index)
    {
        executeReaction(index, cell);
    }, [this, &cell] () {
        initializePropensities(cell);
    });
}

/* ************************************************************************ */

void IntracellularReactions::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
{
    extendIntracellular(ids_plus, ids_minus, rate);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
