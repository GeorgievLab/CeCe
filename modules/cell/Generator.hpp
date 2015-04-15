
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Cell generator module.
 */
class Generator : public simulator::Module
{


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt     Simulation time step.
     * @param world  World object.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
