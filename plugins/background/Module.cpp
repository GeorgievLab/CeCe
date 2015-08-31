/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// Simulation
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"

#if ENABLE_RENDER
#  include "render/Color.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace background {

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    using namespace cv;

    // Get image file path
    const auto imagePath = config.buildFilePath(config.get("image"));

    // Load image
    m_img = imread(imagePath.string());

    if (m_img.empty())
        throw InvalidArgumentException("Cannot open source image: " + imagePath.string());

    // Flip image
    flip(m_img, m_img, 0);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    // Returns grid size
    const auto size = m_img.size();

    if (!m_drawable)
    {
        const Size texSize(size.width, size.height);

        m_drawable.create(context, texSize);

        // Update data
        for (auto&& c : range(texSize))
        {
            const auto pix = m_img.at<cv::Vec3b>(c.getY(), c.getX());
            m_drawable->set(c, render::Color{pix[2] / 255.f, pix[1] / 255.f, pix[0] / 255.f});
        }
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawable->draw(context);
    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
