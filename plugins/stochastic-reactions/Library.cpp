/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author:                                                                  */  
/* Václav Pelíšek <pelisekv@students.zcu.cz>                                */
/* Jiří Fatka <fatkaj@ntis.zcu.cz>                                          */
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
#include "simulator/PluginApi.hpp"
#include "core/Log.hpp"

// Reactions
#include "Reaction.hpp"
#include "ReactionParser.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

class StochasticReactionsApi : public simulator::PluginApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        return ReactionParser().parseReactionCode(code);
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(stochastic_reactions, StochasticReactionsApi)

/* ************************************************************************ */