
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <istream>
#include <stdexcept>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

using Exception = std::runtime_error;

/* ************************************************************************ */

/**
 * @brief Parse source stream and create a new simulation.
 *
 * @param source Source stream
 *
 * @return Simulation from stream.
 */
std::unique_ptr<simulator::Simulation> fromStream(std::istream& source);

/* ************************************************************************ */

}

/* ************************************************************************ */
