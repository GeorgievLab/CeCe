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

// Declaration
#include "Module.hpp"

// C++
#include <cstdio>

// OpenCV
#include <opencv2/highgui/highgui.hpp>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace picture {

/* ************************************************************************ */

void Module::loadConfig(simulator::Simulation& simulation, const simulator::Configuration& config)
{
    // Configure parent
    simulator::Module::loadConfig(simulation, config);

    // File name pattern
    setFilePattern(config.get("pattern", getFilePattern()));

    // Save iteration
    setSaveIteration(config.get("iteration", getSaveIteration()));
}

/* ************************************************************************ */

void Module::update(simulator::Simulation& simulation, units::Time dt)
{
    const auto stepNumber = simulation.getIteration();

    // Skip first image, because it's not rendered yet
    if (stepNumber <= 1)
        return;

    // Skip steps
    if (stepNumber % getSaveIteration() != 0)
        return;

    auto pattern = getFilePattern();
    auto pos = pattern.find("$1");

    // Replace placeholder
    if (pos != String::npos)
        pattern.replace(pos, 2, std::to_string(stepNumber));

    FilePath filename;

// Use working directory on Mac OS X
#if __APPLE__ && __MACH__
    // Get working directory
    const char* dir = getenv("WORKING_DIR");

    // Add directory
    if (dir)
        filename /= dir;
#endif

    filename /= pattern;

    // Write image
    save(filename);

    Log::info("Image saved: ", pattern);
}

/* ************************************************************************ */

void Module::draw(const simulator::Simulation& simulation, render::Context& context)
{
    // Skip first image, because it's not rendered yet
    if (simulation.getIteration() == 0)
        return;

#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    const auto size = context.getSize();

    // Get image data
    m_image = cv::Mat(size.getHeight(), size.getWidth(), CV_8UC4);
    context.getData(m_image.data, true, true);
}

/* ************************************************************************ */

void Module::save(const FilePath& filename)
{
    cv::flip(m_image, m_image, 0);
    cv::imwrite(filename.string(), m_image);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
