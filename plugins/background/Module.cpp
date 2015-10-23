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

// OpenCV
#include <opencv2/highgui/highgui.hpp>

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

void Module::loadConfig(simulator::Simulation& simulation, const simulator::Configuration& config)
{
    // Configure parent
    simulator::Module::loadConfig(simulation, config);

    using namespace cv;

    // Get image file path
    const auto imagePath = config.buildFilePath(config.get("image"));

    // Load image
    //m_img = imread(imagePath.string(), cv::IMREAD_UNCHANGED);
    m_img = imread(imagePath.string(), -1);

    if (m_img.empty())
        throw InvalidArgumentException("Cannot open source image: " + imagePath.string());

    // Flip image
    flip(m_img, m_img, 0);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(const simulator::Simulation& simulation, render::Context& context)
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
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
