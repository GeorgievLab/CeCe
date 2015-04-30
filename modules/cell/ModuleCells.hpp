
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
 * @brief Module that register cell builders.
 */
class ModuleCells : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit ModuleCells(simulator::Simulation& simulation);

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
