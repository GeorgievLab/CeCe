/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// C++
#include <iostream>

// GTest
#include <gtest/gtest.h>

// Plugin
#include "../ReactionsParser.hpp"
#include "../IntracellularReactions.hpp"

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
static void test_impl(int line, const String& code, std::initializer_list<String> names,
    std::initializer_list<float> rates)
{
    std::cout << "@" << line << ": `" << code << "`\n";

    // Parse code
    auto reaction = ReactionsParser<IntracellularReactions>(code).parse();

    // Reaction IDs
    ASSERT_EQ(names.size(), reaction.getMoleculeCount());

    for (auto it = names.begin(); it != names.end(); ++it)
        EXPECT_EQ(*it, reaction.getMoleculeName(it - names.begin()));

    // Reaction rates
    ASSERT_EQ(rates.size(), reaction.getRateCount());

    for (auto it = rates.begin(); it != rates.end(); ++it)
        EXPECT_EQ(*it, reaction.getRate(it - rates.begin()));

    // Reaction table
    ASSERT_EQ(rates.size(), reaction.getReactionCount());
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
    std::cout << "@" << line << ": `" << code << "`\n";

    // Parse code
    auto reaction = ReactionsParser<IntracellularReactions>(code).parse();

    // Reaction IDs
    EXPECT_EQ(0, reaction.getMoleculeCount());

    // Reaction rates
    ASSERT_EQ(0, reaction.getRateCount());

    // Reaction table
    ASSERT_EQ(0, reaction.getReactionCount());
}

/* ************************************************************************ */

/**
 * @brief Macros that hides passing line number.
 */
#define test(...) test_impl(__LINE__, __VA_ARGS__)
#define test_invalid(...) test_invalid_impl(__LINE__, __VA_ARGS__)

/* ************************************************************************ */

TEST(Parser, empty)
{
    test("", {}, {});
    test(";", {}, {});
}

/* ************************************************************************ */

TEST(Parser, expression)
{
    // Single expression
    test(
        "null > 0.453 > A;",
        {"A"},
        {0.453}
    );

    // Multiple expressions
    test(
        "null > 0.1 > A;"
        "null > 0.2 > B;",
        {"A", "B"},
        {0.1, 0.2}
    );
}

/* ************************************************************************ */

TEST(Parser, extract) // Can't remember the right name
{
    test(
        "A > 0.1 > null;"
        "B0 > 0.3 > null;",
        {"A", "B0"},
        {0.1, 0.3}
    );
}

/* ************************************************************************ */

TEST(Parser, assembly)
{
    test(
        "A +B\n> 0.5\t> C;\n"
        "D + A + N32 > 1 > A61;",
        {"A", "B", "C", "D", "N32", "A61"},
        {0.5, 1}
    );
}

/* ************************************************************************ */

TEST(Parser, disassembly)
{
    test(
        "A > 0.5 > B + XNa0;",
        {"A", "B", "XNa0"},
        {0.5}
    );
}

/* ************************************************************************ */

TEST(Parser, arrow)
{
    test(
        "A -> 0.5 -> B;",
        {"A", "B"},
        {0.5}
    );

    test(
        "A > 0.5 -> B;",
        {"A", "B"},
        {0.5}
    );

    test(
        "A -> 0.5 > B;",
        {"A", "B"},
        {0.5}
    );
}

/* ************************************************************************ */

TEST(Parser, newline)
{
    test(
        "A > 0.5 > B;\n",
        {"A", "B"},
        {0.5}
    );

    test(
        "A\n+ C\n ->0.5\n -> B\n;\n",
        {"A", "C", "B"},
        {0.5}
    );
}

/* ************************************************************************ */

TEST(Parser, invalid)
{
    // Missing semicolon
    // TODO: reaction not stored?
    test_invalid("A > 0.5 > B");

    // Molecule name as reaction rate
    // TODO: variable name?
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
}

/* ************************************************************************ */

TEST(Parser, code1)
{
    test("C<0.3, 0.6>B;", {"C", "B"}, {0.6, 0.3});
}

/* ************************************************************************ */

TEST(Parser, code2)
{
    test("A > 10 > B;", {"A", "B"}, {10});
}

/* ************************************************************************ */

TEST(Parser, conditional)
{
    test(
        "if A: A > 0.3 > B;",
        {"A", "B"},
        {0.3}
    );

    test(
        "if C and D: A > 0.3 > B;",
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
}

/* ************************************************************************ */
