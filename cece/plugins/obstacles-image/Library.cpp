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

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/Configuration.hpp"
#include "cece/simulator/Obstacle.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class ObstaclesImageApi : public PluginApi
{
    void configure(Simulation& simulation, const Configuration& config) override
    {
        static constexpr int THRESHOLD = 127;

        using namespace cv;

        // Get image file path
        const auto path = config.buildFilePath(config.get("image"));

        // Load obstacle image
        Mat img = imread(path.string());

        if (img.empty())
            throw InvalidArgumentException("Cannot open source image: " + path.string());

        // Flip image
        flip(img, img, 0);

        // Remove colors
        cvtColor(img, img, CV_BGR2GRAY);

        // To Black & White
        img = img > THRESHOLD;

        // Detect contours
        {
            // Get contours
            DynamicArray<DynamicArray<Point>> contoursSrc;
            DynamicArray<Vec4i> hierarchy;
            findContours(img, contoursSrc, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

            // For each contour compute aproximate version
            DynamicArray<DynamicArray<Point>> contours(contoursSrc.size());

            for (std::size_t i = 0u; i < contoursSrc.size(); ++i)
            {
                approxPolyDP(contoursSrc[i], contours[i], 1, true);
            }

            const auto size = core::Vector<unsigned>(img.size().width, img.size().height);

            // Coordinate scale
            const auto scale = simulation.getWorldSize() / size;
            const auto off = size * 0.5;

            // Create obstacle body
            auto obstacle = simulation.createObject<simulator::Obstacle>();
            auto& shapes = obstacle->getMutableShapes();
            shapes.reserve(contours.size());

            for (const auto& cnt : contours)
            {
                simulator::ShapeEdges shape;

                for (std::size_t i = 0u; i < cnt.size(); ++i)
                {
                    const core::Vector<float> p(cnt[i].x, cnt[i].y);
                    shape.edges.push_back((p - off) * scale);
                }

                shapes.push_back(std::move(shape));
            }

            obstacle->initShapes();
        }
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(obstacles_image, ObstaclesImageApi)

/* ************************************************************************ */
