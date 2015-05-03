
// Declaration
#include "Module.hpp"

// Simulation
#include "core/Log.hpp"
#include "simulator/Simulation.hpp"
#include "parser/Parser.hpp"

// Module
#include "formats.hpp"

/* ************************************************************************ */

namespace module {
namespace picture {

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config)
{
    // File name pattern
    {
        auto filename = config.getString("filename");
        if (!filename.empty())
            setFileName(std::move(filename));
    }

    // Each step
    {
        auto each = config.getInteger("each");
        if (each > 0)
            setEachStep(each);
    }
}

/* ************************************************************************ */

void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    const auto stepNumber = simulation.getStepNumber();

    // Skip first image, because it's not rendered yet
    if (stepNumber == 0)
        return;

    // Skip steps
    if (stepNumber % getEachStep() != 0)
        return;

    auto filename = getFileName();
    auto pos = filename.find("$1");

    // Replace placeholder
    if (pos != std::string::npos)
        filename.replace(pos, 2, std::to_string(stepNumber));

    // Store current image (front buffer)
    if (!save_image(context, filename))
    {
        Log::warning("Unable to write image as: ", filename);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
