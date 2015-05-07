
#ifndef _SIMULATOR_WORLD_FACTORY_H_
#define _SIMULATOR_WORLD_FACTORY_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <string>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;
class Simulator;

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
     * @param simulator
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> createSimulation(Simulator& simulator) const;


    /**
     * @brief Create a new simulation from source file.
     *
     * @param simulator
     * @param filename
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> fromFile(Simulator& simulator, const std::string& filename) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param simulator
     * @param source
     *
     * @return
     */
    virtual std::unique_ptr<Simulation> fromSource(Simulator& simulator, const std::string& source, const std::string& filename = "<source>") const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_FACTORY_H_
