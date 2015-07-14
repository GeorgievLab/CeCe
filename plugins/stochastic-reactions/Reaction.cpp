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

#include "Reaction.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

void Reaction::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step)
{
    m_simulation = &simulation;

    // initialize cell
    if (!object.is<plugin::cell::CellBase>())
        throw RuntimeException("Only object type cell is allowed to have a reaction.");
    cell = object.cast<plugin::cell::CellBase>();

    // initialize random generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0, 1);
    std::discrete_distribution<> distr(propensities.begin(), propensities.end());

    // initialize time units and propenties
    units::Duration time = units::Duration(0);
    if (propensities.size() == 0)
    {
        initializePropensities();
    }

    // Gillespie algorithm
    float sum = std::accumulate(propensities.begin(), propensities.end(), 0.0f);
    if (sum == 0)
        return;
    // tau-leaping
    while(time < step)
    {
        // time of reaction
        auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
        time -= delta_time;
        // which reaction happened
        int index = distr(gen);
        // execute reaction
        executeReaction(index);
    }
}

/* ************************************************************************ */

void Reaction::initializePropensities()
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        propensities.push_back(computePropensity(i));
    }
}

/* ************************************************************************ */

void Reaction::refreshPropensities(unsigned int index)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_rules[i][index].requirement != 0)
        {
            propensities[i] = computePropensity(i);
        }
    }
}

/* ************************************************************************ */

/*bool Reaction::operator ==(const Reaction& rhs)
{
    if (rhs.m_rates.size() != m_rates.size() || rhs.m_ids.size() != m_ids.size())
        return false;
    DynamicArray<unsigned int> skip;
    bool valid;
    for (unsigned int i = 0; i < m_rates.size(); i++)
    {
        valid = false;
        for (unsigned int j = 0; j < rhs.m_rates.size(); i++)
        {
            if (std::find(skip.begin(), skip.end(), j) != skip.end())
                continue;
            if (areEqualRules(rhs, i, j))
            {
                valid = true;
                skip.push_back(j);
                break;
            }
            continue;
        }
        if (!valid)
            return false;
    }
    return true;
}*/

/* ************************************************************************ */

}
}

/* ************************************************************************ */
