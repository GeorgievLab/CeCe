
// Declaration
#include "Module.hpp"

// Magick++
#include <Magick++.h>

// Simulation
#include "core/Log.hpp"
#include "simulator/Simulation.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace picture {

/* ************************************************************************ */

Module::Module()
{
    //InitializeMagick(*argv);
}

/* ************************************************************************ */

Module::~Module()
{

}

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

    // Get pixel data
    auto data = context.getData();
    const auto& imageData = data.first;
    auto imageSize = data.second;

    Magick::Image img(Magick::Geometry(imageSize.getWidth(), imageSize.getHeight()));

    for (unsigned i = 0; i < imageSize.getWidth(); ++i)
    {
        for (unsigned j = 0; j < imageSize.getHeight(); ++j)
        {
            auto base = 3 * (i + imageSize.getWidth() * j);
            img.pixelColor(i, j, Magick::ColorRGB(
                imageData[base] / 255.f,
                imageData[base + 1] / 255.f,
                imageData[base + 2] / 255.f
            ));
        }
    }

    // Write image
    img.write(filename);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
