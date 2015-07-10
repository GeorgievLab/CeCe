/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// GTest
#include <gtest/gtest.h>

// Simulator
#include "core/String.hpp"
#include "core/Tokenizer.hpp"

/* ************************************************************************ */

TEST(Tokenizer, expression)
{
    {
        const String str = "10 13.f + - *";
        ExpressionTokenizer<String::const_iterator> tokenizer(str);

        auto it = tokenizer.begin();
        ASSERT_EQ("10", *it);

        ++it;
        ASSERT_EQ("13.f", *it);

        ++it;
        ASSERT_EQ("+", *it);

        ++it;
        ASSERT_EQ("-", *it);

        ++it;
        ASSERT_EQ("*", *it);

        ++it;
        ASSERT_EQ(tokenizer.end(), it);
    }
}

/* ************************************************************************ */
