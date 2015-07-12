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

#pragma once

#include "ReactionParser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

class ReactionParserIntracellular: public ReactionParser
{
private:

    void check_push(String& id, DynamicArray<String>& array) override;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */