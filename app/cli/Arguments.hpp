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

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

// CLI config
#include "config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/FilePath.hpp"
#include "cece/core/Parameters.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/core/TriBool.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

/**
 * @brief Application parameters.
 */
struct Arguments
{
    /// If a list of available plugins should be printed.
    bool printPlugins = false;

    /// Print help.
    bool printHelp = false;

    /// A list of plugin directories.
    DynamicArray<FilePath> pluginsDirectories;

    /// Path to simulation file.
    FilePath simulationFile;

    /// Simulation parameters.
    Parameters parameters;

#ifdef CECE_ENABLE_RENDER
    // If simulation should be rendered.
    TriBool visualize = cece::Indeterminate;

    /// Window width.
    unsigned int windowWidth = 0;

    /// Window height.
    unsigned int windowHeight = 0;

    // Visualization window in fullscreen mode.
    bool fullscreen = false;
#endif

#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
    /// Path to output video file.
    FilePath videoFileName;
#endif


    /**
     * @brief Parse arguments.
     *
     * @param argc
     * @param argv
     *
     * @return Arguments
     */
    static Arguments parse(int argc, char** argv);

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
