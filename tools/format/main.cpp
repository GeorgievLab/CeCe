/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// C++
#include <iostream>

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/Exception.hpp"
#include "cece/plugin/Manager.hpp"
#include "cece/loader/Loader.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

/**
 * @brief Error function.
 *
 * @param err Error message.
 */
[[noreturn]] void error(const String& err)
{
    std::cerr << err << std::endl;
    exit(1);
}

/* ************************************************************************ */

#ifndef DIR_PLUGINS
/**
 * @brief Returns plugins directory.
 *
 * @param app Executable path.
 * @param dir Directory to plugins.
 *
 * @return
 */
String getPluginsDirectory(FilePath app, FilePath dir) noexcept
{
    return (app.remove_filename() / dir).string();
}
#endif

/* ************************************************************************ */

/**
 * @brief Entry function.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char** argv)
{
    try
    {
        // Parse parameters
        if (argc < 3)
            error("not enough arguments: <infile> <outfile>");

        const FilePath infile = argv[1];
        const FilePath outfile = argv[2];

        auto& manager = cece::plugin::Manager::s();
#ifdef DIR_PLUGINS
        manager.addDirectory(DIR_PLUGINS);
#elif __linux__
        manager.addDirectory(getPluginsDirectory(argv[0], "../lib/cece/plugins"));
#elif __APPLE__ && __MACH__
        manager.addDirectory(getPluginsDirectory(argv[0], "../plugins"));
#elif _WIN32
        manager.addDirectory(getPluginsDirectory(argv[0], "."));
#endif
        // Load initial loader
        manager.load("xml");

        auto& context = manager.getContext();

        // Create simulation
        auto simulation = context.createSimulation(infile);

        // File extension
        auto ext = outfile.extension().string().substr(1);

        // Find loader by extension
        auto loader = context.getLoaderFactoryManager().createLoader(ext);

        if (!loader)
            throw RuntimeException("Unable to store file with extension: '" + ext + "'");

        // Write simulation
        loader->toFile(*simulation, outfile);
    }
    catch (const Exception& e)
    {
        error(e.what());
    }
}

/* ************************************************************************ */
