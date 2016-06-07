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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <iostream>
#include <csignal>
#include <fstream>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Atomic.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/plugin/Manager.hpp"

// CLI
#include "version.hpp"
#include "Arguments.hpp"
#include "Simulator.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/// Global simulate flag
AtomicBool g_simulate{true};

/* ************************************************************************ */

/**
 * @brief Terminate simulation.
 *
 * @param param
 */
void terminate_simulation(int param)
{
    std::cerr << "Terminating simulation" << std::endl;
    g_simulate = false;
}

/* ************************************************************************ */

/**
 * @brief Prints help message.
 *
 * @param name Application name.
 */
[[noreturn]] static void printHelp(const char* name) noexcept
{
    auto bname = FilePath(name).filename();

    std::cout <<
        APP_NAME "\n"
        "\n"
        "  Cell-Cell interaction simulator.\n"
        "\n"
        "Version:\n"
        "    " VERSION "\n"
        "\n"
        "Usage:\n"
        "  " << bname << " "
            "[ --plugins "
            "| --plugins-dir "
            "| --param | -p "
#ifdef CECE_ENABLE_RENDER
            "| --visualize "
            "| --novisualize "
            "| --fullscreen "
            "| --width <width> "
            "| --height <height> "
#endif
#ifdef CONFIG_CLI_ENABLE_VIDEO_CAPTURE
            "| --capture <filename> "
#endif
            "] "
        "<simulation-file>\n"
        "\n"
        "    --plugins            Prints a list of available plugins.\n"
        "    --plugins-dir <dir>  Directory where plugins are located.\n"
        "    --param <name> <value> Set simulation parameter.\n"
        "    -p <name> <value>    Set simulation parameter.\n"
#ifdef CECE_ENABLE_RENDER
        "    --visualize          Enable visualization (default).\n"
        "    --fullscreen         Visualization window in fullscreen mode.\n"
        "    --novisualize        Disable visualization.\n"
        "    --width <width>      Visualization window width "
            "(default " XSTR(CONFIG_CLI_VISUALIZE_WIDTH) "; fullscreen by monitor).\n"
        "    --height <height>    Visualization window height "
            "(default " XSTR(CONFIG_CLI_VISUALIZE_HEIGHT) "; fullscreen by monitor).\n"
#endif
#ifdef CONFIG_CLI_ENABLE_VIDEO_CAPTURE
        "    --capture <filename> Capture video of the simulation.\n"
#endif
        "    <simulation-file>    Path to simulation file.\n"
    << std::endl;

    exit(1);
}

/* ************************************************************************ */

/**
 * @brief Prints a list of available plugins.
 *
 * @param directories Plugin directories.
 */
[[noreturn]] static void printPlugins(DynamicArray<FilePath> directories) noexcept
{
    plugin::Manager manager;
    manager.addDirectories(std::move(directories));

    std::cout <<
        "Plugins directories:\n";

    for (auto name : manager.getDirectories())
        std::cout << "    " << name << "\n";

    std::cout << std::endl;

    std::cout <<
        "Plugins:\n";

    for (auto name : manager.getNames())
        std::cout << "    " << name << "\n";

    std::cout << std::endl;

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
 * @brief Parse and process arguments.
 *
 * @param argc
 * @param argv
 *
 * @return Arguments
 */
cece::cli::Arguments processArguments(int argc, char** argv)
{
    auto args = cece::cli::Arguments::parse(argc, argv);

    // Add default plugin directories
#ifdef DIR_PLUGINS
    args.pluginsDirectories.push_back(DIR_PLUGINS);
#elif __linux__
    args.pluginsDirectories.push_back(getPluginsDirectory(argv[0], "../lib/cece/plugins"));
#elif __APPLE__ && __MACH__
    args.pluginsDirectories.push_back(getPluginsDirectory(argv[0], "../plugins"));
#elif _WIN32
    args.pluginsDirectories.push_back(getPluginsDirectory(argv[0], "."));
#endif

    if (args.printHelp)
        printHelp(argv[0]);

    if (args.printPlugins)
        printPlugins(std::move(args.pluginsDirectories));

    if (args.simulationFile.empty())
        throw InvalidArgumentException("Missing simulation file");

    return args;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

/**
 * @brief Entry function.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char** argv)
{
    // Install signal handler
    signal(SIGTERM, terminate_simulation);
    signal(SIGINT, terminate_simulation);

#if ENABLE_MEASUREMENT
    std::ofstream time_file("time.csv");
    setMeasureTimeOutput(&time_file);
#endif

    try
    {
        // Process arguments
        auto args = processArguments(argc, argv);

        // Create and run simulator
        cece::cli::Simulator(args).start(g_simulate);
    }
    catch (const Exception& e)
    {
        Log::error(e.what());
        return 1;
    }
    catch (...)
    {
        Log::error("Unknown exception");
        return 1;
    }

    // In case of termination, return special code
    return g_simulate ? 0 : 2;
}

/* ************************************************************************ */
