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
#include "ReactionIntracellular.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class ReactionParserIntracellular: public ReactionParser
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
