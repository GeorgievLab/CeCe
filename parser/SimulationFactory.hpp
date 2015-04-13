
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

// Simulator
#include "simulator/SimulationFactory.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

/**
 * @brief WorldLoader interface.
 */
class SimulationFactory : public simulator::SimulationFactory
{

// Public Operations
public:


    /**
     * @brief Create a new world from source file.
     *
     * @param filename
     */
    std::unique_ptr<simulator::Simulation> fromFile(const std::string& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    std::unique_ptr<simulator::Simulation> fromSource(const std::string& source) const override;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
