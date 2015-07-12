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

#include "ReactionParserIntercellular.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

void ReactionParserIntracellular::check_push(String& id, DynamicArray<String>& array)
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
    else if (id == "env" || id == "environment")
    {
        array.push_back("env");
        id.clear();
    }
    else
    {
        array.push_back(id);
        id.clear();
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */