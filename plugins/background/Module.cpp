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
    // Configure parent
    simulator::Module::configure(config, simulation);

    using namespace cv;

    // Get image file path
    const auto imagePath = config.buildFilePath(config.get("image"));

    // Load image
    m_img = imread(imagePath.string(), cv::IMREAD_UNCHANGED);

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
        const bool rgba = m_img.elemSize() == 4;

        m_drawable.create(context, texSize);

        // Update data
        for (auto&& c : range(texSize))
        {
            render::Color pixel;

            if (rgba)
            {
                const auto pix = m_img.at<cv::Vec4b>(c.getY(), c.getX());
                pixel = render::Color::fromUchar(pix[2], pix[1], pix[0], pix[3]);
            }
            else
            {
                const auto pix = m_img.at<cv::Vec3b>(c.getY(), c.getX());
                pixel = render::Color::fromUchar(pix[2], pix[1], pix[0]);
            }

            m_drawable->set(c, pixel);
        }
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    context.colorPush();
    context.enableAlpha();
    m_drawable->draw(context);
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
