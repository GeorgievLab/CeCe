/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// C++
#include <iostream>

// GTest
#include <gtest/gtest.h>

// Module
#include "../Reaction.hpp"

/* ************************************************************************ */

TEST(ReactionTest, test1)
{
    Reaction reaction;

    // A + B > 0.3 > C
    reaction.extend({"A", "B"}, {"C"}, 0.3);

    EXPECT_TRUE(reaction.containsMolecule("A"));
    EXPECT_TRUE(reaction.containsMolecule("B"));
    EXPECT_TRUE(reaction.containsMolecule("C"));
    EXPECT_EQ(1, reaction.getNumberOfReactions());
}

/* ************************************************************************ */

TEST(ReactionTest, test2)
{

}

/* ************************************************************************ */
