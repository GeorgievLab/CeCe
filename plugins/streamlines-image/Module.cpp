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

// Simulation
#include "core/DynamicArray.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"

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

    // Returns grid size.
    const auto size = getLattice().getSize();

    // Image name
    const auto imageName = config.get("image");
    const auto obstacleName = config.get("obstacle", imageName);

    // Get image file path
    const auto imagePath = config.buildFilePath(imageName);
    const auto obstaclePath = config.buildFilePath(obstacleName);

    // Load image
    m_img = imread(imagePath.string());

    if (m_img.empty())
        throw InvalidArgumentException("Cannot open source image: " + imagePath.string());

    // Flip image
    flip(m_img, m_img, 0);

    // Load obstacle image
    Mat imgObstacle = imread(obstaclePath.string());

    if (imgObstacle.empty())
        throw InvalidArgumentException("Cannot open source image: " + obstaclePath.string());

    // Flip image
    flip(imgObstacle, imgObstacle, 0);

    // Resize image to grid size
    resize(imgObstacle, imgObstacle, cv::Size(size.getX(), size.getY()));

    // Remove color
    cvtColor(imgObstacle, imgObstacle, CV_BGR2GRAY);

    // To Black & White
    imgObstacle = imgObstacle > THRESHOLD;

    // Setup obstacle map
    using plugin::streamlines::ObstacleMap;
    ObstacleMap map(size);

    assert(!imgObstacle.empty());

    // Convert image to obstacle map
    for (auto&& c : range(size))
        map[c] = imgObstacle.at<uchar>(c.getY(), c.getX()) > THRESHOLD;

    setStaticObstacleMap(map);
/*
    // Get contours
    DynamicArray<DynamicArray<Point>> contoursSrc;
    DynamicArray<Vec4i> hierarchy;
    findContours(imgObstacle, contoursSrc, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    // For each contour compute aproximate version
    DynamicArray<DynamicArray<Point>> contours(contoursSrc.size());

    for (std::size_t i = 0u; i < contoursSrc.size(); ++i)
    {
        approxPolyDP(contoursSrc[i], contours[i], 20, true);
    }
*/
    // TODO setup
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    using namespace cv;

    // Returns grid size.
    const auto size = 2 * getLattice().getSize();

    if (!m_drawable)
    {
        m_drawable.create(context, size);

        // Resize image to requires size
        resize(m_img, m_img, cv::Size(size.getX(), size.getY()));
    }

    // Update data
    for (auto&& c : range(size))
    {
        const auto pix = m_img.at<Vec3b>(c.getY(), c.getX());

        m_drawable->set(c, render::Color{pix[2] / 255.f, pix[1] / 255.f, pix[0] / 255.f});
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    m_drawable->draw(context);
    context.matrixPop();

    plugin::streamlines::Module::draw(context, simulation);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
