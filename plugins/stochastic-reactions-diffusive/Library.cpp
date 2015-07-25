/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
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
        using namespace plugin::stochastic_reactions;
        using namespace plugin::stochastic_reactions_diffusive;

        return parseReactions<IntercellularReactions>(code, simulation.getParameters());
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(stochastic_reactions_diffusive, StochasticReactionsDiffusiveApi)

/* ************************************************************************ */
