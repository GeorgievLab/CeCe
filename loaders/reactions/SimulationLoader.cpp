/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "SimulationLoader.hpp"

// C++
#include <cassert>

// Simulator
#include "core/Log.hpp"
#include "core/String.hpp"
#include "core/StringStream.hpp"
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"

// Simulator
#include "Configuration.hpp"

/* ************************************************************************ */

namespace loader {
namespace reactions {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Parse directive.
 *
 * @param directive
 * @param simulation
 */
void parseDirective(const StringView& directive, simulator::Simulation& simulation)
{
    InStringStream iss(directive.getData());

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
}

/* ************************************************************************ */

}

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationLoader::fromStream(
    InStream& source, const FilePath& filename) const
{
    String code;
    String line;

    auto simulation = makeUnique<simulator::Simulation>();

    // Read all lines
    while (std::getline(source, line))
    {
        if (line.empty())
            continue;

        if (line.front() == '@')
            parseDirective(StringView(line.c_str() + 1), *simulation);
        else
            code += line;
    }

    // Get plugin
    auto api = simulation->requirePlugin("stochastic-reactions");

    // Create simple static object
    auto object = simulation->buildObject("cell.Yeast", false);

    // Create program
    auto program = api->createProgram(*simulation, "__local__", code);

    // Add program to object
    if (program)
        object->addProgram(program);

    return simulation;
}

/* ************************************************************************ */

void SimulationLoader::toStream(OutStream& os, const simulator::Simulation& simulation, const FilePath& filename) const
{
    // TODO: implement
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
