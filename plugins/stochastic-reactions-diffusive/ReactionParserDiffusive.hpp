#pragma once

#include "Reaction.hpp"
#include "core/DynamicArray.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"
#include "core/Range.hpp"

class ReactionParser
{
    IteratorRange<const char*> range;
    const String whitespace = " \n\r\t\v\b";
    bool validator;
    bool reversible;

    void check_push(String& id, DynamicArray<String>& array);

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
