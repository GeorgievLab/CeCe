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

// Declaration
#include "Arguments.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/FilePath.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

Arguments Arguments::parse(int argc, char** argv)
{
    Arguments args;

    // Foreach arguments
    for (int i = 1; i < argc; ++i)
    {
        const StringView arg(argv[i]);

        // Switch
        if (arg[0] == '-')
        {
            if (arg == "-p" || arg == "--param")
            {
                // Parameter
                if (i + 2 >= argc)
                    throw InvalidArgumentException("Missing parameter value");

                // Store parameter
                args.parameters[argv[i + 1]] = argv[i + 2];
                i += 2;
            }
#ifdef CECE_ENABLE_RENDER
            else if (arg == "--visualize")
            {
                args.visualize = true;
            }
            else if (arg == "--novisualize")
            {
                args.visualize = false;
            }
            else if (arg == "--fullscreen")
            {
                args.fullscreen = true;
            }
            else if (arg == "--width")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("Missing width value");

                args.windowWidth = atoi(argv[++i]);

                if (!args.windowWidth)
                    throw InvalidArgumentException("Invalid width value");
            }
            else if (arg == "--height")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("Missing height value");

                args.windowHeight = atoi(argv[++i]);

                if (!args.windowHeight)
                    throw InvalidArgumentException("Invalid height value");
            }
            else
#endif
#ifdef CECE_CLI_ENABLE_VIDEO_CAPTURE
            if (arg == "--capture")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("Missing filename value");

                args.videoFileName = argv[++i];
            }
            else
#endif
            if (arg == "--plugins-dir")
            {
                if (i + 1 >= argc)
                    throw InvalidArgumentException("Missing directory path");

                args.pluginsDirectories.push_back(argv[++i]);
            }
            else if (arg == "--plugins")
            {
                args.printPlugins = true;
            }
            else if (arg == "--help")
            {
                args.printHelp = true;
            }
        }
        else if (args.simulationFile.empty())
        {
            args.simulationFile = FilePath(arg.getData());
        }
        else
        {
            Log::warning("Unknown argument: ", arg.getData());
        }
    }

    return args;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
