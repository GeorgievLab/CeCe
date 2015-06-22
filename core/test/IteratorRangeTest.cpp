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
#include "core/Range.hpp"
#include "core/DynamicArray.hpp"

/* ************************************************************************ */

TEST(IteratorRangeTest, size)
{
    {
        DynamicArray<int> array;
        IteratorRange<DynamicArray<int>::iterator> range(std::begin(array), std::end(array));
        EXPECT_TRUE(range.isEmpty());
        EXPECT_EQ(0, range.getSize());
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
        EXPECT_EQ(13, range.getSize());
    }

    {
        auto array = DynamicArray<int>{10, 15, 30, 0};
        auto range = makeRange(array);
        EXPECT_FALSE(range.isEmpty());
        EXPECT_EQ(4, range.getSize());
    }
}

/* ************************************************************************ */

TEST(IteratorRangeTest, advance)
{
    {
        auto array = DynamicArray<int>{10, 15, 30, 0};
        auto range = makeRange(array);
        EXPECT_FALSE(range.isEmpty());
        ASSERT_EQ(4, range.getSize());
        EXPECT_EQ(10, range.front());
        range.advanceBegin();
        ASSERT_EQ(3, range.getSize());
        EXPECT_EQ(15, range.front());
        range.advanceBegin();
        ASSERT_EQ(2, range.getSize());
        EXPECT_EQ(30, range.front());
        range.advanceBegin();
        ASSERT_EQ(1, range.getSize());
        EXPECT_EQ(0, range.front());
        range.advanceBegin();
        EXPECT_EQ(0, range.getSize());
        EXPECT_TRUE(range.isEmpty());
    }
}

/* ************************************************************************ */

TEST(IteratorRangeTest, range)
{
    {
        auto rng = range(1, 10);
        EXPECT_FALSE(rng.isEmpty());
        ASSERT_EQ(9, rng.getSize());
    }

    {
        auto rng = range(0, 1000000);
        EXPECT_FALSE(rng.isEmpty());
        ASSERT_EQ(1000000, rng.getSize());
    }
}

/* ************************************************************************ */
