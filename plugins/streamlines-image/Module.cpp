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
#include <cassert>

// OpenCV
#include <opencv2/opencv.hpp>

// Simulation
#include "core/DynamicArray.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Obstacle.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines_image {

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    static constexpr int THRESHOLD = 127;

    using namespace cv;

    // Parent's
    plugin::streamlines::Module::configure(config, simulation);

    // Image name
    const auto imageName = config.get("image");
    const auto obstacleName = config.get("obstacle", imageName);

    // Get image file path
    const auto imagePath = config.buildFilePath(imageName);
    const auto obstaclePath = config.buildFilePath(obstacleName);

    // Load obstacle image
    Mat imgObstacle = imread(obstaclePath.string());

    if (imgObstacle.empty())
        throw InvalidArgumentException("Cannot open source image: " + obstaclePath.string());

    // Flip image
    flip(imgObstacle, imgObstacle, 0);

    // Remove color
    cvtColor(imgObstacle, imgObstacle, CV_BGR2GRAY);

    // To Black & White
    imgObstacle = imgObstacle > THRESHOLD;

    // Detect contours
    {
        auto img = imgObstacle.clone();

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
        const auto off = size / 2;

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

/* ************************************************************************ */

}
}

/* ************************************************************************ */
