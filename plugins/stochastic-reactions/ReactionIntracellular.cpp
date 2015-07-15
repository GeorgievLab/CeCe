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

#include "ReactionIntracellular.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

float ReactionIntracellular::computePropensity(const unsigned int index)
{
    float local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[index].size(); i++)
    {
        if (m_rules[index][i].requirement == 0u)
            continue;
        if (m_rules[index][i].requirement > cell->getMoleculeCount(m_ids[i]))
            return 0;
        local *= cell->getMoleculeCount(m_ids[i]);
    }
    return local;
}

/* ************************************************************************ */

void ReactionIntracellular::executeReaction(const unsigned int index)
{
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change != 0)
        {
            cell->addMolecules(m_ids[i], change);
            refreshPropensities(i);
        }
    }
}

/* ************************************************************************ */

void ReactionIntracellular::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const float rate)
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
            continue;
        }
        array[index].product += 1;
    }
    m_rates.push_back(rate);
    m_rules.push_back(array);
}

/* ************************************************************************ */

int ReactionIntracellular::getIndexOf(const String& id)
{
    auto pointer = std::find(m_ids.begin(), m_ids.end(), id);
    if (pointer == m_ids.end())
    {
        for (unsigned int i = 0; i < m_rules.size(); i++)
        {
            m_rules[i].emplace_back();
        }
        m_ids.push_back(id);
        return m_ids.size() - 1;
    }
    return std::distance(m_ids.begin(), pointer);
}

/* ************************************************************************ */

/* bool ReactionIntracellular::areEqualRules(const ReactionIntracellular& rhs, unsigned int index1, unsigned int index2)
{
    if (m_rates[index1] != rhs.m_rates[index2])
        return false;
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        auto pointer = std::find(rhs.m_ids.begin(), rhs.m_ids.end(), m_ids[i]);
        if (pointer == rhs.m_ids.end())
            return false;
        unsigned int index = std::distance(rhs.m_ids.begin(), pointer);
        if (rhs.m_ids[index] != m_ids[i] || rhs.m_rules[index2][index] != m_rules[index1][i])
            return false;
    }
    return true;
} */

/* ************************************************************************ */

}
}

/* ************************************************************************ */
