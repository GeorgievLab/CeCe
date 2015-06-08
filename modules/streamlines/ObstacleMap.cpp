
/* ************************************************************************ */

// Declaration
#include "ObstacleMap.hpp"

// C++
#include <cassert>
#include <fstream>
#include <stdexcept>

// Magick++
#include <Magick++.h>

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

ObstacleMap ObstacleMap::createFromFile(const std::string& filename, const Vector<unsigned int>& size)
{
    const auto width = size.getWidth();
    const auto height = size.getHeight();

    Magick::Image image;

    // Read a file into image object
    image.read(filename);
    // Resize image
    image.resize(Magick::Geometry(width, height));
    //image.sample(Magick::Geometry(width, height));

    // To black & white
    image.threshold(0.5);

    // Create a map
    ObstacleMap map(size);

    // Get image data
    std::vector<char> pixels(width * height);
    // Image should be black & white only, so get only one component value
    image.write(0, 0, width, height, "R", Magick::CharPixel, pixels.data());

    assert(pixels.size() == (width * height));

    using std::begin;
    using std::end;
    std::transform(begin(pixels), end(pixels), begin(map), [](char c) {
        return !c;
    });

    return map;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
