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
#include "simulator/PluginApi.hpp"
#include "simulator/Simulation.hpp"

// Simulator
#include "Configuration.hpp"

/* ************************************************************************ */

namespace loader {
namespace reactions {

/* ************************************************************************ */

UniquePtr<simulator::Simulation> SimulationLoader::fromStream(
    InStream& source, const FilePath& filename) const
{
    String code;
    String line;

    // Read all lines
    while (std::getline(source, line))
        code += line;

    auto simulation = makeUnique<simulator::Simulation>();
    simulation->setWorldSize(SizeVector(units::um(500)));

    // Get plugin
    auto api = simulation->requirePlugin("stochastic-reactions-diffusive");

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
