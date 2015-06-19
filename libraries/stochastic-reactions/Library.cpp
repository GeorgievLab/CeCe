
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
#include "Reaction.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

Reaction par(String code)
{
    return Reaction();
}

class StochasticReactionsApi : public simulator::LibraryApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        return par(code);
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(stochastic_reactions, StochasticReactionsApi)

/* ************************************************************************ */