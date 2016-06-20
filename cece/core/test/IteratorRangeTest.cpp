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
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(IteratorRangeTest, size)
{
    {
        DynamicArray<int> array;
        IteratorRange<DynamicArray<int>::iterator> range(std::begin(array), std::end(array));
        EXPECT_TRUE(range.isEmpty());
        EXPECT_EQ(0u, range.getSize());
    }

    {
        const char value[] = "Hello world";
        IteratorRange<const char*> range(std::begin(value), std::end(value));
        EXPECT_FALSE(range.isEmpty());
        EXPECT_EQ(sizeof(value), range.getSize());
    }
}

/* ************************************************************************ */

TEST(IteratorRangeTest, makeRange)
{
    {
        auto range = makeRange("Hello world!");
        EXPECT_FALSE(range.isEmpty());
        EXPECT_EQ(13u, range.getSize());
    }

    {
        auto array = DynamicArray<int>{10, 15, 30, 0};
        auto range = makeRange(array);
        EXPECT_FALSE(range.isEmpty());
        EXPECT_EQ(4u, range.getSize());
    }
}

/* ************************************************************************ */

TEST(IteratorRangeTest, advance)
{
    {
        auto array = DynamicArray<int>{10, 15, 30, 0};
        auto range = makeRange(array);
        EXPECT_FALSE(range.isEmpty());
        ASSERT_EQ(4u, range.getSize());
        EXPECT_EQ(10, range.front());
        range.advanceBegin();
        ASSERT_EQ(3u, range.getSize());
        EXPECT_EQ(15, range.front());
        range.advanceBegin();
        ASSERT_EQ(2u, range.getSize());
        EXPECT_EQ(30, range.front());
        range.advanceBegin();
        ASSERT_EQ(1u, range.getSize());
        EXPECT_EQ(0, range.front());
        range.advanceBegin();
        EXPECT_EQ(0u, range.getSize());
        EXPECT_TRUE(range.isEmpty());
    }
}

/* ************************************************************************ */

TEST(IteratorRangeTest, range)
{
    {
        auto rng = range(1, 10);
        EXPECT_FALSE(rng.isEmpty());
        ASSERT_EQ(9u, rng.getSize());
    }

    {
        auto rng = range(0, 1000000);
        EXPECT_FALSE(rng.isEmpty());
        ASSERT_EQ(1000000u, rng.getSize());
    }
}

/* ************************************************************************ */
