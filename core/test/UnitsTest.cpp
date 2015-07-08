/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// GTest
#include <gtest/gtest.h>

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

TEST(UnitsTest, lengthValues)
{
    EXPECT_EQ(units::m(1), units::mm(1000));
    EXPECT_EQ(units::m(0.5), units::mm(500));
    EXPECT_EQ(units::mm(1000), units::m(1));
    EXPECT_EQ(units::m(1), units::mm(1000));
    //EXPECT_EQ(units::mm(5), units::um(5000)); // FIXME: float precision issue
    //EXPECT_EQ(units::um(1), units::mm(0.001));
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
