
// Declaration
#include "Module.hpp"

// Simulation
#include "core/Log.hpp"
#include "simulator/Simulation.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace plugin {
namespace picture {

/* ************************************************************************ */

Module::Module()
{
    //if (!args.empty())
    //    Magick::InitializeMagick(args[0].c_str());
    //else
    //    Log::warning("Variable args is empty");
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config)
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

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    const auto stepNumber = simulation.getIteration();

    // Skip first image, because it's not rendered yet
    if (stepNumber <= 1)
        return;

    // Skip steps
    if (stepNumber % getEachStep() != 0)
        return;

    auto filename = getFileName();
    auto pos = filename.find("$1");

    // Replace placeholder
    if (pos != std::string::npos)
        filename.replace(pos, 2, std::to_string(stepNumber));

    // Write image
    m_image.write(filename);

    Log::info("Image saved: ", filename);
}

/* ************************************************************************ */

void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    // Skip first image, because it's not rendered yet
    if (simulation.getIteration() == 0)
        return;

    // Get pixel data
    auto data = context.getData();
    const auto& imageData = data.first;
    auto imageSize = data.second;

    // Create image
    m_image = Magick::Image(Magick::Geometry(imageSize.getWidth(), imageSize.getHeight()), Magick::Color());

    for (unsigned i = 0; i < imageSize.getWidth(); ++i)
    {
        for (unsigned j = 0; j < imageSize.getHeight(); ++j)
        {
            auto base = 3 * (i + imageSize.getWidth() * j);
            m_image.pixelColor(i, j, Magick::ColorRGB(
                imageData[base] / 255.f,
                imageData[base + 1] / 255.f,
                imageData[base + 2] / 255.f
            ));
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
