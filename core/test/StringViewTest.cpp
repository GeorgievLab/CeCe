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
#include "core/StringView.hpp"

/* ************************************************************************ */

TEST(StringView, construct)
{
    {
        const char* str = "String literal";
        auto view = StringView(str);
        EXPECT_EQ(14, view.getLength());
        EXPECT_EQ(str, view.getData());
    }

    {
        auto str = String("String container");
        auto view = StringView(str);
        EXPECT_EQ(16, view.getLength());
        EXPECT_EQ(str.c_str(), view.getData());
    }
}

/* ************************************************************************ */

TEST(StringView, compare)
{
    {
        auto view = StringView("Hello World!");
        EXPECT_EQ("Hello World!", view);
    }

    {
        auto str = String("Hello World!");
        auto view = StringView(str);
        EXPECT_EQ("Hello World!", view);
    }
}

/* ************************************************************************ */
