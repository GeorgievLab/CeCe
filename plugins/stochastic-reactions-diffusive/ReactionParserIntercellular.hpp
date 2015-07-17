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

#include "../stochastic-reactions/ReactionParser.hpp"
#include "IntercellularReactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class ReactionParserIntercellular: public stochastic_reactions::ReactionsParser
{
using ReactionsParserReactionsParserer;

protected:

    void check_push(String& id, DynamicArray<String>& array) override;

public:

    simulator::Program parse() override;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
