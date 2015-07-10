#include "ReactionParser.hpp"

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

void ReactionParser::skipComments()
{
    while(!range.isEmpty() && (range.front() != '\n' || range.front() != '\r'))
    {
        range.advanceBegin();
    }
}

DynamicArray<String> ReactionParser::parseList()
{
    if (!validator)
        return DynamicArray<String> ();
    DynamicArray<String> array;
    String id;
    while (!range.isEmpty())
    {
        if (range.front() == '#')
        {
            range.advanceBegin();
            skipComments();
        }
        else if (range.front() == ';' || range.front() == '>')
        {
            check_push(id, array);
            range.advanceBegin();
            return array;
        }
        else if (range.front() == '<')
        {
            check_push(id, array);
            range.advanceBegin();
            reversible = true;
            return array;
        }
        else if (range.front() == '+')
        {
            check_push(id, array);
            range.advanceBegin();
        }
        else if (range.front() <= ' ')
        {
            range.advanceBegin();
        }
        else
        {
            id += range.front();
            range.advanceBegin();
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
    if (!range.isEmpty())
    {
        float rate;
        try
        {
            rate = parseExpression(range, {});
        }
        catch (const ExpressionParserException& ex)
        {
            validator = false;
            Log::warning(ex.what());
            return 0;
        }
        if (range.front() == end_char)
        {
            range.advanceBegin();
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

Reaction ReactionParser::parse()
{
    Reaction reaction;
    while (!range.isEmpty())
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
