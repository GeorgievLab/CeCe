/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/IteratorRange.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/Parser.hpp"
#include "cece/core/Parameters.hpp"

/* ************************************************************************ */

namespace cece {
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
float parseExpressionRef(IteratorRange<const char*>& range, const Parameters& parameters = Parameters{});

/* ************************************************************************ */

/**
 * @brief Parse expression from given range of iterators.
 *
 * @param range      Iterator range.
 * @param parameters Map of variables.
 *
 * @return Result value.
 */
inline float parseExpression(IteratorRange<const char*> range, const Parameters& parameters = Parameters{})
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
inline float parseExpression(const StringView& expression, const Parameters& parameters = Parameters{})
{
    return parseExpression(makeRange(
        expression.getData(), expression.getData() + expression.getLength()
    ), parameters);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
