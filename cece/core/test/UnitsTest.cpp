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

// CeCe
#include "cece/core/StringStream.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/UnitSymbol.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(UnitsTest, exponentToCoefficient)
{
    EXPECT_FLOAT_EQ(1e0, units::exponentToCoefficient(0));

    EXPECT_FLOAT_EQ(1e1, units::exponentToCoefficient(1));
    EXPECT_FLOAT_EQ(1e2, units::exponentToCoefficient(2));
    EXPECT_FLOAT_EQ(1e3, units::exponentToCoefficient(3));
    EXPECT_FLOAT_EQ(1e4, units::exponentToCoefficient(4));
    EXPECT_FLOAT_EQ(1e5, units::exponentToCoefficient(5));
    EXPECT_FLOAT_EQ(1e6, units::exponentToCoefficient(6));
    EXPECT_FLOAT_EQ(1e7, units::exponentToCoefficient(7));
    EXPECT_FLOAT_EQ(1e8, units::exponentToCoefficient(8));

    EXPECT_FLOAT_EQ(1e-1, units::exponentToCoefficient(-1));
    EXPECT_FLOAT_EQ(1e-2, units::exponentToCoefficient(-2));
    EXPECT_FLOAT_EQ(1e-3, units::exponentToCoefficient(-3));
    EXPECT_FLOAT_EQ(1e-4, units::exponentToCoefficient(-4));
    EXPECT_FLOAT_EQ(1e-5, units::exponentToCoefficient(-5));
    EXPECT_FLOAT_EQ(1e-6, units::exponentToCoefficient(-6));
    EXPECT_FLOAT_EQ(1e-7, units::exponentToCoefficient(-7));
    EXPECT_FLOAT_EQ(1e-8, units::exponentToCoefficient(-8));
}

/* ************************************************************************ */

TEST(UnitsTest, lengthValues)
{
    EXPECT_EQ(units::m(1), units::mm(1000));
    EXPECT_EQ(units::m(0.5), units::mm(500));
    EXPECT_EQ(units::mm(1000), units::m(1));
    EXPECT_EQ(units::m(1), units::mm(1000));
    //EXPECT_EQ(units::mm(5), units::um(5000)); // FIXME: float precision issue
    //EXPECT_EQ(units::um(1), units::mm(0.001)); // FIXME: float precision issue
}

/* ************************************************************************ */

TEST(UnitsTest, timeValues)
{
    EXPECT_EQ(units::s(1), units::ms(1000));
    EXPECT_EQ(units::ms(500), units::s(0.5));
}

/* ************************************************************************ */

TEST(UnitsTest, massValues)
{
    EXPECT_EQ(units::kg(1), units::g(1000));
    EXPECT_EQ(units::g(1), units::mg(1000));
    EXPECT_EQ(units::mg(1), units::ug(1000));
    EXPECT_EQ(units::ug(1), units::ng(1000));
    EXPECT_EQ(units::ng(1), units::pg(1000));
}

/* ************************************************************************ */

TEST(UnitsTest, multiplication)
{
    EXPECT_EQ(units::m(1) * units::m(1), units::m2(1));
    EXPECT_EQ(units::m(2) * units::m(2), units::m2(4));
    EXPECT_EQ(units::m(3) * units::m(4), units::m2(12));
    EXPECT_EQ(units::mm(2) * units::mm(3), units::mm2(6));
}

/* ************************************************************************ */

TEST(UnitsTest, dividion)
{
    EXPECT_FLOAT_EQ(units::m(1) / units::m(1), 1);
    EXPECT_FLOAT_EQ(units::m(5) / units::m(5), 1);
    EXPECT_FLOAT_EQ(units::m(6) / units::m(2), 3);
    EXPECT_EQ(units::m(8) / units::s(2), units::m_s(4));
}

/* ************************************************************************ */

TEST(UnitsTest, symbol)
{
    using namespace units;

    {
        using type = Unit<List<BaseLength>, List<>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m", symbol);
    }

    {
        using type = Unit<List<BaseLength, BaseLength>, List<>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m2", symbol);
    }

    {
        using type = Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m3", symbol);
    }

    {
        using type = Unit<List<BaseLength, BaseLength, BaseLength, BaseLength>, List<>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m4", symbol);
    }

    {
        using type = Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m2/s", symbol);
    }

    {
        using type = Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
        String symbol(Symbol<type>::type::get().data());
        EXPECT_EQ("m/s2", symbol);
    }
}

/* ************************************************************************ */

TEST(UnitsTest, istream)
{
    {
        InStringStream is("0");

        units::Length val;
        is >> val;

        EXPECT_EQ(units::um(0), val);
    }

    {
        InStringStream is("0um");

        units::Length val;
        is >> val;

        EXPECT_EQ(units::um(0), val);
    }

/*
    // FIXME: Precision issues
    {
        InStringStream is("100um");

        units::Length val;
        is >> val;

        EXPECT_EQ(units::um(100), val);
    }
*/

    {
        InStringStream is("  500ms    ");

        units::Time val;
        is >> val;

        EXPECT_EQ(units::ms(500), val);
    }

    {
        InStringStream is("2um2");

        units::Area val;
        is >> val;

        EXPECT_EQ(units::um2(2), val);
    }

    {
        InStringStream is("1umol/um3");

        units::MolarConcentration val;
        is >> val;

        EXPECT_EQ(units::umol_um3(1), val);
    }
}

/* ************************************************************************ */

TEST(UnitsTest, abbr)
{
    {
        InStringStream is("15mM"); // 1mM = 1mol/m3

        units::MolarConcentration val;
        is >> val;

        EXPECT_EQ(units::mol_m3(15), val);
    }
}

/* ************************************************************************ */

TEST(UnitsTest, parse)
{
    {
        InStringStream is("0");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(0, val);
    }

    {
        InStringStream is("0um");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(0, val);
    }

    {
        InStringStream is("100um");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::um(100).value(), val);
    }

    {
        InStringStream is("  500ms    ");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::ms(500).value(), val);
    }

    {
        InStringStream is("2um2");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::um2(2).value(), val);
    }

    {
        InStringStream is("1umol/um3");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::umol_um3(1).value(), val);
    }

    {
        InStringStream is("15mM");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::mM(15).value(), val);
    }

    {
        InStringStream is("50nM");

        units::Value val = units::parse(is);

        EXPECT_FLOAT_EQ(units::nM(50).value(), val);
    }
}

/* ************************************************************************ */
