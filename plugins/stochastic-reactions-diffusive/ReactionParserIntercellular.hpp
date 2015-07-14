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
#include "ReactionIntercellular.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

class ReactionParserIntercellular: public ReactionParser
{
using ReactionParser::ReactionParser;

protected:

    void check_push(String& id, DynamicArray<String>& array) override;
    
public:

    simulator::Program parse() override;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */