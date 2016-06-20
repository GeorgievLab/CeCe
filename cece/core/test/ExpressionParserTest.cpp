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

// GTest
#include <gtest/gtest.h>

// CeCe
#include "cece/core/IteratorRange.hpp"
#include "cece/core/ExpressionParser.hpp"
#include "cece/core/constants.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(ExpressionParser, basic)
{
    EXPECT_FLOAT_EQ(15.f, parseExpression("15"));
    EXPECT_FLOAT_EQ(15.f, parseExpression("15.f"));
    EXPECT_FLOAT_EQ(10.f, parseExpression("2*5"));
    EXPECT_FLOAT_EQ(4.f, parseExpression("2*6/3"));
    EXPECT_FLOAT_EQ(10.f, parseExpression("2 * 5"));
    EXPECT_FLOAT_EQ(9.f, parseExpression("3 * (1 + 2)"));
    EXPECT_FLOAT_EQ(-3.f, parseExpression("3 * (1 - 2)"));
    EXPECT_FLOAT_EQ(13.f, parseExpression("2 * 5 + 3"));
    EXPECT_FLOAT_EQ(11.f, parseExpression("5 + 2 * 3"));
    EXPECT_FLOAT_EQ(constants::PI, parseExpression("pi"));
    EXPECT_FLOAT_EQ(constants::E, parseExpression("e"));
}

/* ************************************************************************ */

TEST(ExpressionParser, functions)
{
    EXPECT_FLOAT_EQ(3.f, parseExpression("sqrt(9)"));
    EXPECT_FLOAT_EQ(10.f, parseExpression("abs(10)"));
    EXPECT_FLOAT_EQ(10.f, parseExpression("abs(-10)"));
    EXPECT_FLOAT_EQ(0.f, parseExpression("log(1)"));
    EXPECT_FLOAT_EQ(0.f, parseExpression("log    (1)"));
    EXPECT_FLOAT_EQ(0.f, parseExpression("sin(0)"));
    EXPECT_FLOAT_EQ(2.f, parseExpression("ln(E^2)"));
    //EXPECT_FLOAT_EQ(0.f, parseExpression("sin(atan(1)*4)"));
    EXPECT_FLOAT_EQ(1.f, parseExpression("cos(2*pi)"));
}

/* ************************************************************************ */

TEST(ExpressionParser, variables)
{
    EXPECT_FLOAT_EQ(7.f, parseExpression("v1 * v2 + v3", Parameters{{
        {"v1", "5.0"}, {"v2", "2"}, {"v3", "-3"}
    }}));
}

/* ************************************************************************ */

TEST(ExpressionParser, inner)
{
    {
        const char str[] = "15 + 3 hello";
        auto range = makeRange(str);

        EXPECT_FLOAT_EQ(18.f, parseExpressionRef(range));
        EXPECT_EQ(str + 7, range.begin());
    }
}

/* ************************************************************************ */

TEST(ExpressionParser, invalid)
{
    EXPECT_THROW(parseExpression("45-    10 + lol"), UnknownConstantException);
    EXPECT_THROW(parseExpression("sin()"), UnknownConstantException);
    EXPECT_THROW(parseExpression(" 9 * cos ( 5"), MissingParenthesisException);
    EXPECT_THROW(parseExpression(" 9 * cos ( "), UnknownConstantException);
    EXPECT_THROW(parseExpression("5 + (10 * (6)"), MissingParenthesisException);
    EXPECT_THROW(parseExpression("\t\b\b\v"), EmptyExpressionException);
    EXPECT_THROW(parseExpression(" "), EmptyExpressionException);
}

/* ************************************************************************ */
