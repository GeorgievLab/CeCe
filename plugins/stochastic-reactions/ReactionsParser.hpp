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

#include "Reactions.hpp"
#include "core/DynamicArray.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"
#include "core/Tokenizer.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class ReactionsParser
{
protected:

    IteratorRange<const char*> range;
    const String whitespace = " \n\r\t\v\b";
    bool validator;
    bool reversible;

    virtual void check_push(String& id, DynamicArray<String>& array) = 0;

    void skipComments();

    DynamicArray<String> parseList();

    float parseRate(const char end_char);

public:

    virtual simulator::Program parse() = 0;

    ReactionsParser(const String& code) NOEXCEPT
    : range(makeRange(code.c_str(), code.c_str() + code.size()))
    {
        // Nothing to do.
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
