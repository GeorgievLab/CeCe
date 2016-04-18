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

// CeCe
#include "cece/core/UnitIo.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/UnitsCtors.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(VectorUnitsTest, istream)
{
    {
        std::istringstream is("10 20");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(10), vec.getX());
        EXPECT_EQ(units::um(20), vec.getY());
    }

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

    {
        std::istringstream is("0 10um");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(0), vec.getX());
        EXPECT_EQ(units::um(10), vec.getY());
    }

    {
        std::istringstream is("0um 10um");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(0), vec.getX());
        EXPECT_EQ(units::um(10), vec.getY());
    }

    {
        std::istringstream is("5um 0");

        Vector<units::Length> vec;
        is >> vec;

        EXPECT_EQ(units::um(5), vec.getX());
        EXPECT_EQ(units::um(0), vec.getY());
    }
}

/* ************************************************************************ */

TEST(VectorUnitsTest, ostream)
{
    //Vector
}

/* ************************************************************************ */
