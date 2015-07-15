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
#include "core/VectorRange.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

TEST(VectorRangeTest, loop)
{
    {
        // Test range [0, 0] -> (0, 0)
        auto rng = range(Vector<int>(0, 0), Vector<int>(0, 0));

        EXPECT_TRUE(rng.isEmpty());
    }

    {
        // Test range [0, 0] -> (1, 1)
        auto rng = range(Vector<int>(0, 0), Vector<int>(1, 1));
        rng.advanceBegin();

        EXPECT_TRUE(rng.isEmpty());
    }

    {
        // Test range [0, 0] -> (2, 2)
        auto rng = range(Vector<int>(0, 0), Vector<int>(2, 2));

        EXPECT_EQ(Vector<int>(0, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(1, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(0, 1), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(1, 1), rng.front());
        rng.advanceBegin();

        EXPECT_TRUE(rng.isEmpty());
    }

    {
        // Test range [0, 0] -> (5, 3)
        auto rng = range(Vector<int>(0, 0), Vector<int>(5, 3));

        EXPECT_EQ(Vector<int>(0, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(1, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(2, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(3, 0), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(4, 0), rng.front());
        rng.advanceBegin();

        EXPECT_EQ(Vector<int>(0, 1), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(1, 1), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(2, 1), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(3, 1), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(4, 1), rng.front());
        rng.advanceBegin();

        EXPECT_EQ(Vector<int>(0, 2), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(1, 2), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(2, 2), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(3, 2), rng.front());
        rng.advanceBegin();
        EXPECT_EQ(Vector<int>(4, 2), rng.front());
        rng.advanceBegin();

        EXPECT_TRUE(rng.isEmpty());
    }

}

/* ************************************************************************ */
