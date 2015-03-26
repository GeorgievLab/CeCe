
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <istream>
#include <stdexcept>

// Simulator
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

using Exception = std::runtime_error;

/* ************************************************************************ */

/**
 * @brief Parse world from source.
 *
 * @param source
 *
 * @return
 */
std::unique_ptr<simulator::World> fromStream(std::istream& source);

/* ************************************************************************ */

}

/* ************************************************************************ */
