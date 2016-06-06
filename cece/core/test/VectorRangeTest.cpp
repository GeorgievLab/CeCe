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
#include "cece/core/VectorRange.hpp"
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

using namespace cece;

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
