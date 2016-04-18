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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(StringView, construct)
{
    {
        const char* str = "String literal";
        auto view = StringView(str);
        EXPECT_EQ(14u, view.getLength());
        EXPECT_EQ(str, view.getData());
    }

    {
        auto str = String("String container");
        auto view = StringView(str);
        EXPECT_EQ(16u, view.getLength());
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
