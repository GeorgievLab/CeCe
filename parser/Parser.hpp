
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <istream>
#include <stdexcept>

// Simulator
#include "simulator/Simulator.hpp"
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
std::unique_ptr<simulator::Simulation> fromStream(simulator::Simulator& simulator, std::istream& source);

/* ************************************************************************ */

}

/* ************************************************************************ */
