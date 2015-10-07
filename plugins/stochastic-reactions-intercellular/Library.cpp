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

// Simulator
#include "simulator/Plugin.hpp"
#include "simulator/PluginApi.hpp"

// Plugins
#include "plugins/stochastic-reactions-intracellular/ReactionsParser.hpp"

// Plugin
#include "IntercellularReactions.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse directive.
 *
 * @param directive
 * @param simulation
 */
void parseDirective(const StringView& directive, Simulation& simulation,
    Configuration& config)
{
    InStringStream iss(directive.getData() + 1);

    String name;
    iss >> name;

    if (name == "iterations")
    {
        simulator::IterationNumber iterations;
        iss >> iterations;
        simulation.setIterations(iterations);
        Log::info("Number of iterations: ", iterations);
    }
    else if (name == "dt")
    {
        units::Time dt;
        iss >> dt;
        simulation.setTimeStep(dt);
        Log::info("Time step: ", dt);
    }
    else if (name == "molecule")
    {
        String molecule;
        unsigned int amount;
        iss >> molecule >> amount;

        auto sub = config.addConfiguration("molecule");
        sub.set("name", molecule);
        sub.set("amount", amount);
    }
    else if (name == "parameter")
    {
        String param;
        String expression;

        iss >> param;

        // Get rest of line
        std::getline(iss, expression);

        // Parse expression
        simulation.setParameter(param, parseExpression(expression, simulation.getParameters()));
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */

class StochasticReactionsIntercellularApi : public PluginApi
{

    String getLoaderExtension() const noexcept override
    {
        return "reactions";
    }


    UniquePtr<simulator::Simulation> fromStream(InStream& source, const FilePath& filename) const
    {
        String code;
        String line;

        auto simulation = makeUnique<simulator::Simulation>();
        simulation->setVisualize(false);

        // Configuration
        simulator::Configuration config(filename);

        // Read all lines
        while (std::getline(source, line))
        {
            if (line.empty())
                continue;

            if (line.front() == '@')
                parseDirective(line, *simulation, config);
            else
                code += line;
        }

        // Get plugin
        auto api = simulation->requirePlugin("stochastic-reactions-intracellular");

        // Create simple static object
        auto object = simulation->buildObject("cell.Yeast", simulator::Object::Type::Static);

        // Configure cell
        object->configure(config, *simulation);

        // Create program
        auto program = api->createProgram(*simulation, "__local__", code);

        // Add program to object
        if (program)
            object->addProgram(program);

        auto storeProgram = simulation->buildProgram("cell.store-molecules");

        // Add program to object
        if (storeProgram)
            object->addProgram(storeProgram);

        return simulation;
    }


    void toStream(OutStream& os, const Simulation& simulation, const FilePath& filename) const
    {
        // TODO: implement
    }


    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        using namespace plugin::stochastic_reactions;
        using namespace plugin::stochastic_reactions_diffusive;

        return parseReactions<IntercellularReactions>(code, simulation.getParameters());
    }

};

/* ************************************************************************ */

DEFINE_PLUGIN(stochastic_reactions_intercellular, StochasticReactionsIntercellularApi)

/* ************************************************************************ */
