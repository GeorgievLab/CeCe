#include "ReactionParser.hpp"
#include "Reaction.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"

void ReactionParser::check_push(String& id, DynamicArray<String>& array)
{
    if (id.size() == 0)
    {
        validator = false;
        Log::warning("Couldnt parse reaction");
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
    DynamicArray<String> array;
    String id;
    while (current != end_of_string && validator)
    {
        if (*current == ';' || *current == '>')
        {
            check_push(id, array);
            ++current;
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
    return array;
}

float ReactionParser::parseRate()
{
    if (!validator)
        return 0;
    if (current != end_of_string)
    {
        char* end;
        float rate = strtof(current, &end);
        if (current == end)
        {
            validator = false;
            Log::warning("Reaction has invalid rate.");
            return 0;
        }
        current = end;
        while (*current == ' ' || *current == '\n' || *current == '\r' || *current == '\t' || *current == '\v')
        {
            ++current;
        }
        if (*current == '>')
        {
            ++current;
            return rate;
        }
        validator = false;
        Log::warning("Reaction rate area has no end.");
        return 0;
    }
    validator = false;
    Log::warning("Reaction has no reaction rate.");
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
        auto ids_minus = parseList();
        float rate = parseRate();
        auto ids_plus = parseList();
        if (validator)
        {
            reaction.extend(ids_plus, ids_minus, rate);
        }
    }
    return reaction;
}
