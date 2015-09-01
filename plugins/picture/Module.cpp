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
#include "Module.hpp"

// C++
#include <cstdio>

// Simulation
#include "core/Log.hpp"
#include "core/UniquePtr.hpp"
#include "core/Exception.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace picture {

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // File name pattern
    setFilePattern(config.get("pattern", getFilePattern()));

    // Save iteration
    setSaveIteration(config.get("iteration", getSaveIteration()));
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
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

    // Write image
    save(pattern);

    Log::info("Image saved: ", pattern);
}

/* ************************************************************************ */

void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    // Skip first image, because it's not rendered yet
    if (simulation.getIteration() == 0)
        return;

#if THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

    const auto size = context.getSize();

    // Get image data
    m_image = cv::Mat(size.getWidth(), size.getHeight(), CV_8UC4);
    context.getData(m_image.data, true);
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

/* ************************************************************************ */
