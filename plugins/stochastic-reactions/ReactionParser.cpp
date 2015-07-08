#include "ReactionParser.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"

void ReactionParser::check_push(String& id, DynamicArray<String>& array)
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

DynamicArray<String> ReactionParser::parseList()
{
    if (!validator)
        return DynamicArray<String> ();
    DynamicArray<String> array;
    String id;
    while (current != end_of_string)
    {
        if (*current == ';' || *current == '>')
        {
            check_push(id, array);
            ++current;
            return array;
        }
        if (*current == '<')
        {
            check_push(id, array);
            ++current;
            reversible = true;
            return array;
        }
        else if (*current == '+')
        {
            check_push(id, array);
            ++current;
        }
        else if(*current == ' ' || *current == '\n' || *current == '\r' || *current == '\t' || *current == '\v')
        {
            ++current;
        }
        else
        {
            id += *current;
            ++current;
        }
    }
    validator = false;
    Log::warning("Reaction has no end. This may be caused by missing semicolon.");
    return DynamicArray<String> ();
}

float ReactionParser::parseRate(const char end_char)
{
    if (!validator)
        return 0;
    if (current != end_of_string)
    {
        auto range = makeRange(current, end_of_string);
        float rate = parseExpression(range, {});
        if (*current == end_char)
        {
            ++current;
            return rate;
        }
        validator = false;
        Log::warning("Rate area is either invalid or has no end.");
        return 0;
    }
    validator = false;
    Log::warning("Reaction has no rate area.");
    return 0;
}

Reaction ReactionParser::parseReactionCode(const String& code)
{
    Reaction reaction;
    current = code.c_str();
    end_of_string = current + code.length();
    while (current != end_of_string)
    {
        validator = true;
        reversible = false;
        auto ids_minus = parseList();
        float rate;
        float rateR;
        if (reversible)
        {
            rate = parseRate(',');
            rateR = parseRate('>');
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
    return reaction;
}
