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
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines_image {

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    using namespace cv;

    // Parent's
    plugin::streamlines::Module::configure(config, simulation);

    // Returns grid size.
    auto size = getLattice().getSize();

    // Get image file path
    auto path = config.buildFilePath(config.get("image"));

    // Load image
    Mat imgOrig = imread(path.string(), CV_LOAD_IMAGE_GRAYSCALE);

    if (imgOrig.data == NULL)
        throw InvalidArgumentException("Cannot open source image: " + path.string());

    // Resize image to grid size
    Mat imgResized;
    resize(imgOrig, imgResized, cv::Size(size.getX(), size.getY()));

    // To Black & White
    Mat img = imgResized > 127;

    // Setup obstacle map
    using plugin::streamlines::ObstacleMap;
    ObstacleMap map(size);

    assert(img.data);

    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            map[ObstacleMap::CoordinateType(i, j)] = img.at<uchar>(i, j);
        }
    }

    setStaticObstacleMap(map);

    // Get contours
    DynamicArray<DynamicArray<Point>> contoursSrc;
    DynamicArray<Vec4i> hierarchy;
    findContours(img, contoursSrc, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    // For each contour compute aproximate version
    DynamicArray<DynamicArray<Point>> contours(contoursSrc.size());

    for (std::size_t i = 0u; i < contoursSrc.size(); ++i)
    {
        approxPolyDP(contoursSrc[i], contours[i], 20, true);
    }

    // TODO setup
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
