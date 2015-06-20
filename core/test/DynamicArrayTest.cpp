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
#include "core/DynamicArray.hpp"

/* ************************************************************************ */

TEST(DynamicArrayTest, size)
{
    DynamicArray<int> array;
    EXPECT_EQ(0, array.size());

    array.resize(10);
    EXPECT_EQ(10, array.size());
}

/* ************************************************************************ */
