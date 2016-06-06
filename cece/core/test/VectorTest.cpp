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

// C++
#include <sstream>
#include <complex>

// CeCe
#include "cece/core/Vector.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(VectorTest, zero)
{
    const Vector<float> vec{Zero};

    EXPECT_FLOAT_EQ(0, vec.getX());
    EXPECT_FLOAT_EQ(0, vec.getY());
}

/* ************************************************************************ */

TEST(VectorTest, accessors)
{
    const Vector<float> vec{2, 3};

    EXPECT_FLOAT_EQ(2.0, vec.getX());
    EXPECT_FLOAT_EQ(3.0, vec.getY());
}

/* ************************************************************************ */

TEST(VectorTest, comparisonOperators)
{
    const Vector<float> v1{10, 50};
    const Vector<float> v2{60, 30};
    const Vector<float> v3{10, 50};
    const Vector<float> v4{10, 80};
    const Vector<float> v5{10, 10};

    // Same vectors
    EXPECT_TRUE(v1 == v1);
    EXPECT_TRUE(v2 == v2);
    EXPECT_TRUE(v3 == v3);
    EXPECT_TRUE(v4 == v4);
    EXPECT_TRUE(v5 == v5);

    // Same vector values
    EXPECT_TRUE(v1 == v3);
    EXPECT_TRUE(v3 == v1);

    // Different vectors
    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v2 != v1);
    EXPECT_TRUE(v2 != v3);
    EXPECT_TRUE(v3 != v2);
    EXPECT_TRUE(v4 != v1);
    EXPECT_TRUE(v4 != v2);
    EXPECT_TRUE(v4 != v3);
    EXPECT_TRUE(v4 != v5);
}

/* ************************************************************************ */

TEST(VectorTest, addition)
{
    const Vector<float> v1{6.2, 3.3};
    const Vector<float> v2{1.4, 0.9};

    const auto vRes0 = v1 + v2;

    EXPECT_FLOAT_EQ(7.6, vRes0.getX());
    EXPECT_FLOAT_EQ(4.2, vRes0.getY());

    const auto vRes1 = v2 + v1;

    EXPECT_FLOAT_EQ(7.6, vRes1.getX());
    EXPECT_FLOAT_EQ(4.2, vRes1.getY());
}

/* ************************************************************************ */

TEST(VectorTest, substraction)
{
    const Vector<float> v1{6.2, 3.3};
    const Vector<float> v2{1.4, 0.9};

    const auto vRes0 = v1 - v2;

    EXPECT_FLOAT_EQ(4.8, vRes0.getX());
    EXPECT_FLOAT_EQ(2.4, vRes0.getY());

    const auto vRes1 = v2 - v1;

    EXPECT_FLOAT_EQ(-4.8, vRes1.getX());
    EXPECT_FLOAT_EQ(-2.4, vRes1.getY());
}

/* ************************************************************************ */

TEST(VectorTest, multiplication)
{
    const Vector<float> v1{6.2, 3.3};
    const Vector<float> v2{1.4, 0.9};

    const auto vRes0 = v1 * v2;

    EXPECT_FLOAT_EQ( 8.68, vRes0.getX());
    EXPECT_FLOAT_EQ( 2.97, vRes0.getY());

    const auto vRes1 = v1 * 2.2;

    EXPECT_FLOAT_EQ(13.64, vRes1.getX());
    EXPECT_FLOAT_EQ( 7.26, vRes1.getY());

    const auto vRes2 = 5.1 * v2;

    EXPECT_FLOAT_EQ( 7.14, vRes2.getX());
    EXPECT_FLOAT_EQ( 4.59, vRes2.getY());
}

/* ************************************************************************ */

TEST(VectorTest, division)
{
    const Vector<float> v1{6.2, 3.3};
    const Vector<float> v2{1.4, 0.9};

    const auto vRes0 = v1 / v2;

    EXPECT_FLOAT_EQ(4.428571429, vRes0.getX());
    EXPECT_FLOAT_EQ(3.666666667, vRes0.getY());
}

/* ************************************************************************ */

TEST(VectorTest, dot)
{
    const Vector<float> v1{ 0,  5};
    const Vector<float> v2{10,  0};
    const Vector<float> v3{ 5,  5};
    const Vector<float> v4{ 1,  1};
    const Vector<float> v5{ 2, -2};

    EXPECT_FLOAT_EQ( 0, v1.dot(v2));
    EXPECT_FLOAT_EQ(50, v2.dot(v3));
    EXPECT_FLOAT_EQ( 0, v4.dot(v5));
}

/* ************************************************************************ */

TEST(VectorTest, lengthSquared)
{
    const Vector<float> v1{Zero};
    const Vector<float> v2{1, 1};
    const Vector<float> v3{2, 4};
    const Vector<float> v4{3, 8};

    EXPECT_FLOAT_EQ(0, v1.getLengthSquared());
    EXPECT_FLOAT_EQ(2, v2.getLengthSquared());
    EXPECT_FLOAT_EQ(20, v3.getLengthSquared());
    EXPECT_FLOAT_EQ(73, v4.getLengthSquared());
}

/* ************************************************************************ */

TEST(VectorTest, length)
{
    const Vector<float> v1{Zero};
    const Vector<float> v2{1, 1};
    const Vector<float> v3{2, 4};
    const Vector<float> v4{3, 8};

    EXPECT_FLOAT_EQ(0, v1.getLength());
    EXPECT_FLOAT_EQ(1.414213562, v2.getLength());
    EXPECT_FLOAT_EQ(4.472135955, v3.getLength());
    EXPECT_FLOAT_EQ(8.544003745, v4.getLength());
}

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
