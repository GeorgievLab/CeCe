/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* ************************************************************************ */

// Simulator
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Reactions
#include "../stochastic-reactions/ReactionsParser.hpp"
#include "IntercellularReactions.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

class StochasticReactionsDiffusiveApi : public PluginApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        return plugin::stochastic_reactions::ReactionsParser<plugin::stochastic_reactions_diffusive::IntercellularReactions>(code).parse();
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(stochastic_reactions_diffusive, StochasticReactionsDiffusiveApi)

/* ************************************************************************ */
