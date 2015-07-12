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

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

float ReactionIntercellular::computePropensity(const unsigned int index)
{
    float local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[i].size(); i++)
    {
        if (m_rules[index][i].requirement == 0)
        {
            continue;
        }
        else if (i % 2 == 0)
        {
            local *= cell->getMoleculeCount(m_ids[i]);
        }
        else
        {
            local *= 1;// TODO: počet molekul typu i-1
        }
    }
    return local;
}

void ReactionIntercellular::executeReaction(const unsigned int index)
{
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        int change = m_rules[index][i].product - m_rules[index][i].requirement;
        if (change == 0)
            continue;
        if (i % 2 == 0)
            cell->addMolecules(m_ids[i], change);
        else
            changeMoleculesInEnvironment(m_ids[i-1], change);
        refreshPropensities(i);
    }
}

void ReactionIntercellular::changeMoleculesInEnvironment(const String id, const int change)
{
    
}

void ReactionIntercellular::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const loat rate)
{
    m_rates.push_back(rate);
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
    m_rules.push_back(array);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
