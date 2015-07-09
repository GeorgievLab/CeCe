/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// GTest
#include <gtest/gtest.h>

// Simulator
#include "core/Range.hpp"
#include "core/ExpressionParser.hpp"

/* ************************************************************************ */

TEST(ExpressionParser, basic)
{
    EXPECT_FLOAT_EQ(15.f, parseExpression("15", {}));
    EXPECT_FLOAT_EQ(15.f, parseExpression("15.f", {}));
    EXPECT_FLOAT_EQ(10.f, parseExpression("2*5", {}));
    EXPECT_FLOAT_EQ(4.f, parseExpression("2*6/3", {}));
    EXPECT_FLOAT_EQ(10.f, parseExpression("2 * 5", {}));
    EXPECT_FLOAT_EQ(9.f, parseExpression("3 * (1 + 2)", {}));
    EXPECT_FLOAT_EQ(-3.f, parseExpression("3 * (1 - 2)", {}));
    EXPECT_FLOAT_EQ(13.f, parseExpression("2 * 5 + 3", {}));
    EXPECT_FLOAT_EQ(11.f, parseExpression("5 + 2 * 3", {}));
}

/* ************************************************************************ */

TEST(ExpressionParser, functions)
{
    EXPECT_FLOAT_EQ(3.f, parseExpression("sqrt(9)", {}));
    EXPECT_FLOAT_EQ(10.f, parseExpression("abs(10)", {}));
    EXPECT_FLOAT_EQ(10.f, parseExpression("abs(-10)", {}));
    EXPECT_FLOAT_EQ(0.f, parseExpression("log(1)", {}));
}

/* ************************************************************************ */

TEST(ExpressionParser, variables)
{
    EXPECT_FLOAT_EQ(7.f, parseExpression("v1 * v2 + v3", {
        {"v1", 5.f}, {"v2", 2.f}, {"v3", -3.f}
    }));
}

/* ************************************************************************ */

TEST(ExpressionParser, inner)
{
    {
        const char str[] = "15 + 3 hello";
        auto range = makeRange(str);

        EXPECT_FLOAT_EQ(18.f, parseExpression(range, {}));
        EXPECT_EQ(str + 8, range.begin());
    }
}

/* ************************************************************************ */
