
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <istream>

// Simulator
#include "core/Vector.hpp"
#include "core/Grid.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Class for loading obstacle map from file.
 */
class ObstacleMap : public core::Grid<unsigned char>
{

// Public Ctors & Dtors
public:


    using core::Grid<unsigned char>::Grid;


// Public Operations
public:


    /**
     * @brief Create obstacle map from input stream.
     *
     * @param filename Source file.
     *
     * @return
     */
    static ObstacleMap createFromFile(const std::string& filename, const Vector<unsigned int>& size);

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
