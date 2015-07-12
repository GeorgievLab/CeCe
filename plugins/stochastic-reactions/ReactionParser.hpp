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

#include "Reaction.hpp"
#include "core/DynamicArray.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"
#include "core/Range.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

class ReactionParser
{
    IteratorRange<const char*> range;
    const String whitespace = " \n\r\t\v\b";
    bool validator;
    bool reversible;
    
    virtual void check_push(String& id, DynamicArray<String>& array);

    void skipComments();

    DynamicArray<String> parseList();

    float parseRate(const char end_char);
    
public:

    ReactionParser(const String& code) NOEXCEPT
    : range(makeRange(code.c_str(), code.c_str() + code.size()))
    {
        // Nothing to do.
    }

    Reaction parse();
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */