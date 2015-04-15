
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Module for cells.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module();


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation Current simulation.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
