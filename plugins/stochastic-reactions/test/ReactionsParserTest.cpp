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

// C++
#include <iostream>

// GTest
#include <gtest/gtest.h>

// Plugin
#include "plugins/stochastic-reactions/ReactionsParser.hpp"
#include "plugins/stochastic-reactions/Reactions.hpp"
#include "plugins/stochastic-reactions/Context.hpp"
#include "simulator/Simulation.hpp"
#include "plugins/cell/CellBase.hpp"

/* ************************************************************************ */

using namespace plugin::stochastic_reactions;

/* ************************************************************************ */

/**
 * @brief Compare source code with expected reaction values.
 *
 * @param line          Source test line.
 * @param code          Code to parse.
 * @param names         Expected names in code.
 * @param rates         Expected rates in code.
 * @param reactionCount Expected number of reactions.
 */
static void test_impl(int line, const String& code, std::initializer_list<bool> cond, std::initializer_list<String> names,
    std::initializer_list<float> rates)
{
    SCOPED_TRACE(code);

    simulator::Simulation simulation;
    plugin::cell::CellBase cell(simulation);
    Context context = Context(nullptr, cell, nullptr, simulation.getParameters());

    // Parse code
    auto reactions = ReactionsParser(code).parse();

    // Conditions
    ASSERT_EQ(cond.size(), reactions.getReactionCount());

    for (auto it = cond.begin(); it != cond.end(); ++it)
        EXPECT_EQ(*it, reactions.evalCond(it - cond.begin(), context));

    // Molecule names
    ASSERT_EQ(names.size(), reactions.getMoleculeCount());

    for (auto it = names.begin(); it != names.end(); ++it)
        EXPECT_EQ(*it, reactions.getMoleculeName(it - names.begin()));

    // Reaction rates
    ASSERT_EQ(rates.size(), reactions.getReactionCount());

    for (auto it = rates.begin(); it != rates.end(); ++it)
        EXPECT_EQ(*it, reactions.evalRate(it - rates.begin(), context));
}

/* ************************************************************************ */

/**
 * @brief Compare source code with expectation of invalid code.
 *
 * @param line Source test line.
 * @param code Code to parse.
 */
static void test_invalid_impl(int line, const String& code)
{
    SCOPED_TRACE(code);

    // Parse code
    auto reactions = ReactionsParser(code).parse();

    // Reaction IDs
    EXPECT_EQ(0, reactions.getMoleculeCount());

    // Reaction table
    ASSERT_EQ(0, reactions.getReactionCount());
}

/* ************************************************************************ */

/**
 * @brief Macros that hides passing line number.
 */
#define test(...) test_impl(__LINE__, __VA_ARGS__)
#define test_invalid(...) test_invalid_impl(__LINE__, __VA_ARGS__)

/* ************************************************************************ */

TEST(Parser, simple)
{
    test(
        "null > 453 > A;",
        {true},
        {"A"},
        {453}
    );
}

TEST(Parser, decimal_point)
{
    test(
        "null > 0.453 > A;",
        {true},
        {"A"},
        {0.453}
    );
}

TEST(Parser, multiple_expressions)
{
    test(
        "null > 0.1 > A;"
        "null > 0.2 > B;",
        {true, true},
        {"A", "B"},
        {0.1, 0.2}
    );
}

TEST(Parser, inline_expressions)
{
    test(
        "null > 0.1 > A;C > 0.2 > B;T > 0.3 > G;",
        {true, true, true},
        {"A", "C", "B", "T", "G"},
        {0.1, 0.2, 0.3}
    );
}

TEST(Parser, complex_molecule_name)
{
    test(
        "null > 0.1 > A_C56_B;",
        {true},
        {"A_C56_B"},
        {0.1}
    );
}

TEST(Parser, multiline_expressions)
{
    test(
        "A \n> 0.5\t> C;\n"
        "D \n > \t1 > A61;",
        {true, true},
        {"A", "C", "D", "A61"},
        {0.5, 1}
    );

    test(
        "A > 0.5 > B;\n",
        {true},
        {"A", "B"},
        {0.5}
    );

    test(
        "A\n+ C\n ->0.5\n -> B\n;\n",
        {true},
        {"A", "C", "B"},
        {0.5}
    );
}

TEST(Parser, assembly)
{
    test(
        "A +B> 0.5> C;"
        "D + A + N32 > 1 > A61;",
        {true, true},
        {"A", "B", "C", "D", "N32", "A61"},
        {0.5, 1}
    );
}

TEST(Parser, disassembly)
{
    test(
        "A > 0.5 > B + XNa0;",
        {true},
        {"A", "B", "XNa0"},
        {0.5}
    );
}

TEST(Parser, arrow)
{
    test(
        "A -> 0.5 -> B;",
        {true},
        {"A", "B"},
        {0.5}
    );

    test(
        "A > 0.5 -> B;",
        {true},
        {"A", "B"},
        {0.5}
    );

    test(
        "A -> 0.5 > B;",
        {true},
        {"A", "B"},
        {0.5}
    );
}

TEST(Parser, invalid)
{
    // Empty
    test_invalid("");
    test_invalid(";");

    // Molecule name as reaction rate
    test_invalid("A > A > B;");

    // Uncomplete definition
    test_invalid("A");
    test_invalid("A;");
    test_invalid("A\t>;");
    test_invalid("A > B;");
    test_invalid("A > 0.3");
    test_invalid("A > 0.3 >;");
    test_invalid("A > B;");

    // Invalid identifier list
    test_invalid("A + > 0.1 > D;");
    test_invalid(" + > 0.1 > D;");
    test_invalid(" + B > 0.1 > D;");
    test_invalid("A + B + > 0.1 > D;");
    test_invalid("A + B > 0.1 > C +;");
    test_invalid("A + B + C> 0.1 > + D +E;");

    // Missing semicolon
    test_invalid("A + B -> 0.3 > D");
    test_invalid("A + B -> 0.3 -> D + N");
    test_invalid("A + B -> 0.3 -> D + N\nA + O -> 3 > N;");

    // Missing reversible rate
    test_invalid("A < 10 > B;");
}

TEST(Parser, reversible)
{
    test(
        "A  + b < 10, 20 > B;",
        {true, true},
        {"A", "b", "B"},
        {20, 10}
    );

    test(
        "A  + b <- 10, 20 -> B;",
        {true, true},
        {"A", "b", "B"},
        {20, 10}
    );

    test(
        "A  + b < 10, 20 -> B;",
        {true, true},
        {"A", "b", "B"},
        {20, 10}
    );

    test(
        "A  + b <- 10, 20 > B;",
        {true, true},
        {"A", "b", "B"},
        {20, 10}
    );
}

TEST(Parser, rate_expressions)
{
    test(
        "A  + b > 10 + 20 > B;",
        {true},
        {"A", "b", "B"},
        {30}
    );

    test(
        "A  + sin > (10 + 20) * sin(0) -> B;",
        {true},
        {"A", "sin", "B"},
        {0}
    );
}

TEST(Parser, rate_expressions_inner_function)
{
    test(
        "A  + b > 20 * cos(5 - 40 / 8) -> B;",
        {true},
        {"A", "b", "B"},
        {20}
    );
}


/*
TEST(Parser, conditional)
{
    test(
        "if A: A > 0.3 > B;",
        {false},
        {"A", "B"},
        {0.3}
    );

    test(
        "if C and D: A > 0.3 > B;",
        {false},
        {"A", "B", "C", "D"},
        {0.3}
    );

    test(
        "if C and A: A > 0.3 > B;",
        { "A", "B", "C"},
        {0.3}
    );

    // Reaction is inserted twice
    test(
        "if C or D: A > 0.3 > B;",
        {"A", "B", "C", "D"},
        {0.3, 0.3}
    );

    test(
        "if not E or B: A < 0.5, 1 > B;",
        {"A", "B", "E"},
        {1, 1, 0.5, 0.5}
    );

    test(
        "if not E and B: A < 0.3, 0.5 > B;"
        "C > 1.5 > A + B;",
        {"A", "B", "E", "C"},
        {0.5, 0.3, 1.5}
    );

    test(
        "if E and F or G and H and I or not J: A + B + C < 0.3, 0.4 > D;",
        {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"},
        {0.4, 0.4, 0.4, 0.3, 0.3, 0.3}
    );
}

*/
