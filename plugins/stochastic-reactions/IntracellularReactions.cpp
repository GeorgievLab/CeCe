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

#include "IntracellularReactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */
/**
 * @brief Computes propensity of given reaction.
 *
 * @param index of row, cell, diffusion
 * @return propensity
 */
IntracellularReactions::PropensityType IntracellularReactions::computePropensity(const unsigned int index, const plugin::cell::CellBase& cell)
{
    auto local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        unsigned int number = cell.getMoleculeCount(m_moleculeNames[i]);

        // TODO: check if it's OK
        const bool cond =
            // >
            (m_rules[index][i].less && !(number <= m_rules[index][i].requirement)) ||
            // <=
            !(number >= m_rules[index][i].requirement)
        ;

        if (cond)
            return 0;

        if (m_rules[index][i].requirement == 0u)
            continue;
        local *= number;
    }
    return local;
}

/* ************************************************************************ */
/**
 * @brief Computes propensities of all reactions.
 *
 * @param cell, diffusion
 * @return
 */
void IntracellularReactions::initializePropensities(const plugin::cell::CellBase& cell)
{
    m_propensities.clear();
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        m_propensities.push_back(computePropensity(i, cell));
    }
}

/* ************************************************************************ */
/**
 * @brief Refreshes propensities of ractions which have requirements of specific molecule.
 *
 * @param index of column, cell, diffusion
 */
void IntracellularReactions::refreshPropensities(const unsigned int index, const plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_rules[i][index].requirement != 0)
        {
            m_propensities[i] = computePropensity(i, cell);
        }
    }
}

/* ************************************************************************ */
/**
 * @brief Executes reaction which ocuured.
 *
 * @param index of rection, cell, diffusion
 */
void IntracellularReactions::executeReaction(const unsigned int index, plugin::cell::CellBase& cell)
{
    for (unsigned int i = 0; i < m_moleculeNames.size(); i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change != 0)
        {
            cell.addMolecules(m_moleculeNames[i], change);
            refreshPropensities(i, cell);
        }
    }
}

/* ************************************************************************ */
/**
 * @brief Main function of this plugin.
 *
 */
void IntracellularReactions::operator()(simulator::Object& object, simulator::Simulation&, units::Duration step)
{
    auto& cell = object.castThrow<cell::CellBase>();

    executeReactions(step, [this, &cell](unsigned int index)
    {
        executeReaction(index, cell);
    }, [this, &cell] () {
        initializePropensities(cell);
    });
}

/* ************************************************************************ */
/**
 * @brief Function that extends reaction rule matrix with reaction that releases signal outside the cell.
 *
 * @param array of IDs, reation rate
 */
void IntracellularReactions::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
{
    extendIntracellular(ids_plus, ids_minus, rate);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
