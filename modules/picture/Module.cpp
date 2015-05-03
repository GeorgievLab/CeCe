
// Declaration
#include "Module.hpp"

// Simulation
#include "simulator/Simulation.hpp"

// Module
#include "formats.hpp"

/* ************************************************************************ */

namespace module {
namespace picture {

/* ************************************************************************ */

void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    // Skip first image, because it's not rendered yet
    if (simulation.getStepNumber() == 0)
        return;

    // Store current image (front buffer)
    save_image(context, "image_" + std::to_string(simulation.getStepNumber()) + ".png");
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
