/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Range.hpp"
#include "core/Map.hpp"
#include "core/StringView.hpp"
#include "core/Exception.hpp"
#include "core/Parser.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Define specialized expression parser exception.
 *
 * @param className Name of exception class.
 * @param message   Error message.
 */
#define DEFINE_EXPRESSION_EXCEPTION(className, message) \
    DEFINE_PARSER_EXCEPTION_BASE(className, ExpressionParserException, message)

/* ************************************************************************ */

/**
 * @brief Base expression parser exception.
 */
class ExpressionParserException: public ParserException {};

/* ************************************************************************ */

DEFINE_EXPRESSION_EXCEPTION(EmptyExpressionException, "Cannot parse empty expression");
DEFINE_EXPRESSION_EXCEPTION(MissingParenthesisException, "Missing closing parethesis");
DEFINE_EXPRESSION_EXCEPTION(UnknownConstantException, "Unknown constant name");
DEFINE_EXPRESSION_EXCEPTION(UnknownFunctionException, "Unknown function name");

/* ************************************************************************ */

/**
 * @brief Parse expression from given range of iterators.
 *
 * @param range      Iterator range reference - it is updated.
 * @param parameters Map of variables.
 *
 * @return Result value.
 */
float parseExpressionRef(IteratorRange<const char*>& range, const Map<String, float>& parameters = {});

/* ************************************************************************ */

/**
 * @brief Parse expression from given range of iterators.
 *
 * @param range      Iterator range.
 * @param parameters Map of variables.
 *
 * @return Result value.
 */
inline float parseExpression(IteratorRange<const char*> range, const Map<String, float>& parameters = {})
{
    return parseExpressionRef(range, parameters);
}

/* ************************************************************************ */

/**
 * @brief Parse expression from given range of iterators.
 *
 * @param expression Expression string.
 * @param parameters Map of variables.
 *
 * @return Result value.
 */
inline float parseExpression(const StringView& expression, const Map<String, float>& parameters = {})
{
    return parseExpression(makeRange(
        expression.getData(), expression.getData() + expression.getLength()
    ), parameters);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
