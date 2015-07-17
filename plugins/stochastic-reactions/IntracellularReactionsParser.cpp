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

#include "IntracellularReactionsParser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

void IntracellularReactionsParser::check_push(String& id, DynamicArray<String>& array)
{
    if (id.size() == 0)
    {
        validator = false;
        Log::warning("I cannot accept empty molecule name. You may want to use 'null' molecule in degradation/expression reactions.");
    }
    else if (id == "null")
    {
        id.clear();
    }
    else
    {
        array.push_back(id);
        id.clear();
    }
}

/* ************************************************************************ */

simulator::Program IntracellularReactionsParser::parse()
{
    IntracellularReactions reaction;
    while (!range.isEmpty())
    {
        validator = true;
        reversible = false;
        auto ids_minus = parseList();
        float rate;
        float rateR;
        if (reversible)
        {
            rateR = parseRate(',');
            rate = parseRate('>');
        }
        else
        {
            rate = parseRate('>');
        }
        auto ids_plus = parseList();
        if (validator)
        {
            reaction.extend(ids_plus, ids_minus, rate);
            if (reversible)
                reaction.extend(ids_minus, ids_plus, rateR);
        }
    }
    return simulator::Program(reaction);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
