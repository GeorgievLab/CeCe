
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <string>
#include <istream>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/SimulationFactory.hpp"

/* ************************************************************************ */

namespace parser {

/* ************************************************************************ */

/**
 * @brief Parse simulation factory interface.
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
    std::unique_ptr<simulator::Simulation> fromFile(simulator::Simulator& simulator, const std::string& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     */
    std::unique_ptr<simulator::Simulation> fromSource(simulator::Simulator& simulator, const std::string& source) const override;


// Protected Operations
protected:


    /**
     * @brief Parse simulation from stream.
     *
     * @param simulator
     * @param source
     *
     * @return
     */
    virtual std::unique_ptr<simulator::Simulation> fromStream(simulator::Simulator& simulator, std::istream& source) const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
