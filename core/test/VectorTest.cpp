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
