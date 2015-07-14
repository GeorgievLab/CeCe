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

#include "ReactionParser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

void ReactionParser::skipComments()
{
    while(!range.isEmpty() && (range.front() != '\n' || range.front() != '\r'))
    {
        range.advanceBegin();
    }
}

/* ************************************************************************ */

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

/* ************************************************************************ */

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

/* ************************************************************************ */

}
}

/* ************************************************************************ */
