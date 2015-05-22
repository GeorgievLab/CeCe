
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
    std::unique_ptr<simulator::Simulation> fromFile(const std::string& filename) const override;


    /**
     * @brief Create a new world from source code.
     *
     * @param source
     * @param filename
     */
    std::unique_ptr<simulator::Simulation> fromSource(
        const std::string& source, const std::string& filename = "<source>") const override;


// Protected Operations
protected:


    /**
     * @brief Parse simulation from stream.
     *
     * @param source
     * @param filename
     *
     * @return
     */
    virtual std::unique_ptr<simulator::Simulation> fromStream(
        std::istream& source, const std::string& filename) const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
