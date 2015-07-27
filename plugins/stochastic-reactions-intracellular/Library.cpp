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
#include "ReactionsParser.hpp"
#include "IntracellularReactions.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

class StochasticReactionsIntracellularApi : public PluginApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        using namespace plugin::stochastic_reactions;
        return parseReactions<IntracellularReactions>(code, simulation.getParameters());
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(stochastic_reactions_intracellular, StochasticReactionsIntracellularApi)

/* ************************************************************************ */
