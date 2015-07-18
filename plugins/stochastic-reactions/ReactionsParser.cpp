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

#include "ReactionsParser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

DynamicArray<std::pair<String, unsigned int>> parseConditions()
{
    return DynamicArray<String, unsigned int>();
}
    
/* ************************************************************************ */

ReactionsParser::NumberType ReactionsParser::parseRate(TokenCode end)
{
    if (!validator)
        return 0;
    if (!is(TokenCode::Invalid))
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

DynamicArray<String> ReactionsParser::parseMinusList()
{
    if (!validator)
        return DynamicArray<String> ();
    DynamicArray<String> array;
    while (!is(TokenCode::Invalid))
    {
        if (is(TokenCode::Identifier))
        {
            check_push(token().value, array);
            next();
        }
        else if (is(TokenCode::Greater))
        {
            next();
            return array;
        }
        else if (is(TokenCode::Less))
        {
            reversible = true;
            next();
            return array;
        }
        else if (is(TokenCode::Plus))
        {
            next();
        }
        else
        {
            break;
        }
    }
    validator = false;
    Log::warning("Left side  of reaction has no end.");
    return DynamicArray<String> ();
}

/* ************************************************************************ */

DynamicArray<String> ReactionsParser::parsePlusList()
{
    if (!validator)
        return DynamicArray<String> ();
    DynamicArray<String> array;
    while (!is(TokenCode::Invalid))
    {
        if (is(TokenCode::Identifier))
        {
            check_push(token().value, array);
            next();
        }
        else if (is(TokenCode::Semicolon))
        {
            next();
            return array;
        }
        else if (is(TokenCode::Plus))
        {
            next();
        }
        else
        {
            break;
        }
    }
    validator = false;
    Log::warning("Right side of reaction has no end. This may be caused by missing semicolon.");
    return DynamicArray<String> ();
}

/* ************************************************************************ */

void ReactionsParser::check_push(String& id, DynamicArray<String>& array)
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
    else if (id == "environment")
    {
        array.push_back("env");
        id.clear();
    }
    else
    {
        array.push_back(id);
        id.clear();
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
