/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// C++
#include <sstream>
#include <complex>

// Simulator
#include "core/Vector.hpp"

/* ************************************************************************ */

TEST(VectorTest, istream)
{
    {
        std::istringstream is("10 20");

        Vector<int> vec;
        is >> vec;

        EXPECT_EQ(10, vec.getX());
        EXPECT_EQ(20, vec.getY());
    }

    {
        std::istringstream is("0.3 1.1");

        Vector<float> vec;
        is >> vec;

        EXPECT_FLOAT_EQ(0.3, vec.getX());
        EXPECT_FLOAT_EQ(1.1, vec.getY());
    }

    {
        std::istringstream is("(10,15) (90)");

        Vector<std::complex<float>> vec;
        is >> vec;

        EXPECT_EQ(std::complex<float>(10, 15), vec.getX());
        EXPECT_EQ(std::complex<float>(90), vec.getY());
    }

    {
        std::istringstream is("10");

        Vector<int> vec;
        is >> vec;

        EXPECT_EQ(10, vec.getX());
        EXPECT_EQ(10, vec.getY());
    }

    {
        std::istringstream is("0.5");

        Vector<float> vec;
        is >> vec;

        EXPECT_FLOAT_EQ(0.5, vec.getX());
        EXPECT_FLOAT_EQ(0.5, vec.getY());
    }

}

/* ************************************************************************ */

TEST(VectorTest, ostream)
{
    //Vector
}

/* ************************************************************************ */
