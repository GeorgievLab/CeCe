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

// CeCe
#include "cece/core/UnitsCtors.hpp"
#include "cece/plugin/Context.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugins
#include "cece/plugins/cell/CellBase.hpp"

// Plugin
#include "cece/plugins/stochastic-reactions/ReactionsParser.hpp"
#include "cece/plugins/stochastic-reactions/Reactions.hpp"
#include "cece/plugins/stochastic-reactions/Context.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::plugin::stochastic_reactions;

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
static void test_impl(
    int line,
    const String& code,
    std::initializer_list<bool> cond,
    std::initializer_list<String> names,
    std::initializer_list<float> rates,
    std::initializer_list<std::pair<String, int>> molecules = {},
    std::initializer_list<float> propensities = {})
{
    SCOPED_TRACE(code);

    plugin::Context pluginContext;
    simulator::Simulation simulation(pluginContext);
    plugin::cell::CellBase cell(simulation);
    Context context(nullptr, cell, nullptr, simulation.getParameters());

    for (auto pair : molecules)
    {
        cell.changeMoleculeCount(pair.first, pair.second);
    }

    // Parse code
    auto reactions = ReactionsParser(code).parse();
    reactions.initializePropensities(context);

    // Reaction conditions
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
        EXPECT_FLOAT_EQ(*it, reactions.evalRate(it - rates.begin(), context));

    // Optional test - Reaction propensities
    if (propensities.size())
    {
        ASSERT_EQ(propensities.size(), reactions.getPropensityCount());
        ASSERT_EQ(reactions.getPropensityCount(), reactions.getReactionCount());

        for (auto it = propensities.begin(); it != propensities.end(); ++it)
            EXPECT_FLOAT_EQ(*it, reactions.getPropensity(it - propensities.begin()));
    }
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

    Reactions reactions;

    try
    {
        // Parse code
        reactions = ReactionsParser(code).parse();
    }
    catch (const ParserException& ex)
    {
        // Nothing to do.
    }
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

TEST(Parser, invalid)
{
    // Empty
    test_invalid("");
    test_invalid(";");

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

TEST(Parser, basic)
{
    test(
        "null > 453 > A;",
        {true},
        {"A"},
        {453}
    );
}

TEST(Parser, decimal)
{
    test(
        "null > 0.453 > A;",
        {true},
        {"A"},
        {0.453}
    );
}

TEST(Parser, multiple)
{
    test(
        "null > 0.1 > A;"
        "null > 0.2 > B;",
        {true, true},
        {"A", "B"},
        {0.1, 0.2}
    );
}

TEST(Parser, inlineexpr)
{
    test(
        "null > 0.1 > A;C > 0.2 > B;T > 0.3 > G;",
        {true, true, true},
        {"A", "C", "B", "T", "G"},
        {0.1, 0.2, 0.3}
    );
}

TEST(Parser, complexname)
{
    test(
        "null > 0.1 > A_C56_B;",
        {true},
        {"A_C56_B"},
        {0.1}
    );
}

TEST(Parser, multiline)
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

TEST(Parser, reversible_advanced)
{
    test(
        "null < 1, 0.5 \n* 2> C;"
        "D + A +\n N32 < 20, 15 > A61;",
        {true, true, true, true},
        {"C", "D", "A", "N32", "A61"},
        {1, 1, 15, 20}
    );

    test(
        "null < 0.5, 0.6 > B;",
        {true, true},
        {"B"},
        {0.6, 0.5}
    );

    test(
        "null < 0.3, 0.1 > A_C56_B;null < 2 , 5 > A_C56_B;",
        {true, true, true, true},
        {"A_C56_B"},
        {0.1,0.3,5,2}
    );
}

TEST(Parser, reversible_arrow)
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
        "A  + sin > 5 - 40 / 8 -> B;",
        {true},
        {"A", "sin", "B"},
        {0}
    );
}

TEST(Parser, rate_expressions_2)
{
    test(
        "A  + sin > (10 + 20) * sin(0) -> B;",
        {true},
        {"A", "sin", "B"},
        {0}
    );

    test(
        "A  + sin > 20 * cos(0) -> B;",
        {true},
        {"A", "sin", "B"},
        {20}
    );
}

TEST(Parser, rate_expressions_3)
{
    test(
        "A  + b > cos(5 - 40 / 8) -> B;",
        {true},
        {"A", "b", "B"},
        {1}
    );

    test(
        "A  + b > 20 * cos(5 - 40 / 8) -> B;",
        {true},
        {"A", "b", "B"},
        {20}
    );

    test(
        "A  + b > 20 * cos( 5 - 40 / log10( 10 ^ 8 ) ) -> B;",
        {true},
        {"A", "b", "B"},
        {20}
    );
}

TEST(Parser, reversible_rate_expressions)
{
    test(
        "A  + b < cos(5 - 40 / 8), 40 * 9 -> B;",
        {true, true},
        {"A", "b", "B"},
        {360, 1}
    );

    test(
        "A  + b <- sin(ln(1)), 20 * cos(5 - 40 / 8) -> B;",
        {true, true},
        {"A", "b", "B"},
        {20, 0}
    );

    test(
        "A  + b <- 20 * cos( 5 - 40 / log10( 10 ^ 8 ) ), 40+ \n 6 > B;",
        {true, true},
        {"A", "b", "B"},
        {46, 20}
    );
}

TEST(Parser, numberformat)
{
    test(
        "A  + b > 10.68 > B;",
        {true},
        {"A", "b", "B"},
        {10.68}
    );

    test(
        "A  + sin > 10.68E-1 -> B;",
        {true},
        {"A", "sin", "B"},
        {1.068}
    );

    test(
        "A  + sin > 10.68e-1 -> B;",
        {true},
        {"A", "sin", "B"},
        {1.068}
    );

    test(
        "A  + sin > 10.68e+1 -> B;",
        {true},
        {"A", "sin", "B"},
        {106.8}
    );

    test(
        "A  + sin > 10.68E1 -> B;",
        {true},
        {"A", "sin", "B"},
        {106.8}
    );
}

TEST(Parser, reversible_numberformat)
{
    test(
        "A  + b < 10E+2, 2000E-2 > B;",
        {true, true},
        {"A", "b", "B"},
        {20, 1000}
    );
}

TEST(Parser, functions)
{
    test(
        "A  + b > hill(A, 5 ,1) > B;",
        {true},
        {"A", "b", "B"},
        {0.5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "A  + b > A + B + 8 > B;",
        {true},
        {"A", "b", "B"},
        {16},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "A  + C > A * B / C > B;",
        {true},
        {"A", "C", "B"},
        {7.5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "null > 10 > A;"
        "null > A > B;",
        {true, true},
        {"A", "B"},
        {10, 5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, reversible_functions)
{
    test(
        "A  + b < A , B + 1 > B;",
        {true, true},
        {"A", "b", "B"},
        {4, 5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, conditional_basic)
{
    test(
        "if D > 0: A > 0.3 > B;",
        {false},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C > 0 and D > 0: A > 0.3 > B;",
        {false},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C > 0 or D > 0: A > 0.3 > B;",
        {true},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C > 1 and A > 1: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C > 1 and D > 1 or A > 1: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C > 1 and D > 1 or A > 8: A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, negate)
{
    test(
        "A  + b > -hill(A, 5 ,1) > B;",
        {true},
        {"A", "b", "B"},
        {-0.5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "A  + b > 100* (- ln ( 0.5 ) / 20.0) > B;",
        {true},
        {"A", "b", "B"},
        {3.4657359027997265},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "A  + b > 100* - ln ( 0.5 ) / 20.0 > B;",
        {true},
        {"A", "b", "B"},
        {3.4657359027997265},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, conditional_advanced)
{
    test(
        "if D > A: A > 0.3 > B;",
        {false},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if A > C: A > 0.3 > B;",
        {true},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if A != B: A > 0.3 > B;",
        {true},
        {"A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and A: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 5 and 6: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and (B and A): A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and (B and D): A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 3 < A > 4: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 10 > A > 3: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 10 = 3: A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 10 / 8 * 3 > 50 * cos(D): A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if 1 < C < B < 4 < A < 6 < 7 < 8 < 9 < 10: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, keywordnot)
{
    test(
        "if not(1 < C < B < 4 < A < 6 < 7 < 8 < 9 < 10): A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if not 10 = 3: A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if not 10 / 8 * 3 > 50 * cos(D): A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if not 5 and 6: A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and not (B and A): A > 0.3 > B;",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and not (D and A): A > 0.3 > B;",
        {true},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, concentration)
{
/*
    // FIXME: comparision bug
    test(
        "if A > 50nM: null > 0.1 > A;"
        "if A < 50nM: null > 0.2 > B;",
        {false, true},
        {"A", "B"},
        {0.1, 0.2}
    );

    // FIXME: comparision bug
    test(
        "if A = 50nM: null > 0.1 > A;"
        "if B = 50uM: null > 0.2 > B;",
        {true, true},
        {"A", "B"},
        {0.1, 0.2},
        {{"A", units::nM(50).value()}, {"B", units::uM(50).value()}}
    );

    // FIXME: comparision bug
    test(
        "if A <= 50nM: null > 0.1 > A;"
        "if B >= 50uM: null > 0.2 > B;"
        "if A =< 50nM: null > 0.1 > A;"
        "if B => 50uM: null > 0.2 > B;",
        {true, true, true, true},
        {"A", "B"},
        {0.1, 0.2, 0.1, 0.2},
        {{"A", units::nM(50).value()}, {"B", units::uM(50).value()}}
    );

    test(
        "if A != 50nM: null > 0.1 > A;"
        "if B != 50uM: null > 0.2 > B;",
        {false, false},
        {"A", "B"},
        {0.1, 0.2},
        {{"A", units::nM(50).value()}, {"B", units::uM(50).value()}}
    );

    test(
        "if env A > 50nM: null > 0.1 > A;"
        "if env A < 50nM: null > 0.2 > B;",
        {false, true},
        {"A", "B"},
        {0.1, 0.2}
    );
*/
}

TEST(Parser, blockcond)
{
    test(
        "if C and not (B and A):{ A > 0.3 > B;}",
        {false},
        { "A", "B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );

    test(
        "if C and not (B and A):{ A > 0.3 > B;A > 0.4 > B;A > 0.5 > B;}",
        {false, false, false},
        { "A", "B"},
        {0.3, 0.4, 0.5},
        {{"C", 2}, {"A", 5}, {"B", 3}}
    );
}

TEST(Parser, propensities_basic)
{
    test(
        "null > 0.3 * A > B;",
        {true},
        {"B"},
        {1.5},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {1.5}
    );

    test(
        "null > 0.3 > B;",
        {true},
        {"B"},
        {0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {0.3}
    );

    test(
        "A > 0.3 * B > B;",
        {true},
        {"A", "B"},
        {0.9},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {4.5}
    );
}

TEST(Parser, propensities_multiple)
{
    test(
        "null > 0.3 * A > B; null > 0.4 > D;",
        {true, true},
        {"B", "D"},
        {1.5, 0.4},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {1.5, 0.4}
    );

    test(
        "null > 0.3 > B; B > 0.3 > C;",
        {true, true},
        {"B", "C"},
        {0.3, 0.3},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {0.3, 0.9}
    );

    test(
        "A > 0.3 * A > B; B > 0.6 * B > A;",
        {true, true},
        {"A", "B"},
        {1.5, 1.8},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {7.5, 5.4}
    );

    test(
        "null > 5 > A + B; B > 10 > C;",
        {true, true},
        {"A", "B", "C"},
        {5, 10},
        {{"C", 2}, {"A", 5}, {"B", 3}},
        {5, 30}
    );
}
