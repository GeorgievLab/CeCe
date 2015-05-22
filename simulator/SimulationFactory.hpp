
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Simulation factory interface.
 */
class SimulationFactory
{

// Public Dtors
public:


    /**
     * @brief Virtual destructor.
     */
    virtual ~SimulationFactory() {}


// Public Operations
public:


    /**
     * @brief Create empty simulation.
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> createSimulation() const;


    /**
     * @brief Create a new simulation from source file.
     *
     * @param filename
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> fromFile(const std::string& filename) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param source
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> fromSource(const std::string& source, const std::string& filename = "<source>") const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
