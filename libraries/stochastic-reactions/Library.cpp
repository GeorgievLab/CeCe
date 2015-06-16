
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cassert>
#include <cstring>

// Simulator
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"

// Reactions
#include "Degradation.hpp"
#include "Expression.hpp"
#include "Assembly.hpp"
#include "Disassembly.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

class StochasticReactionsApi : public simulator::LibraryApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        if (name == "deg")
            return Degradation();
        else if (name == "exp")
            return Expression();
        else if (name == "asm")
            return Assembly();
        else if (name == "das")
            return Disassembly();
        else
            return {};
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(stochastic_reactions, StochasticReactionsApi)

/* ************************************************************************ */
