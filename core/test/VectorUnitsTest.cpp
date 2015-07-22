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

// Simulator
#include "core/VectorUnits.hpp"

/* ************************************************************************ */

TEST(VectorUnitsTest, istream)
{
    {
        std::istringstream is("10um 20um");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(10), vec.getX());
        EXPECT_EQ(units::um(20), vec.getY());
    }

    {
        std::istringstream is("10um");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(10), vec.getX());
        EXPECT_EQ(units::um(10), vec.getY());
    }
}

/* ************************************************************************ */

TEST(VectorUnitsTest, ostream)
{
    //Vector
}

/* ************************************************************************ */
