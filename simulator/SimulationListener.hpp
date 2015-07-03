/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Event listener class for events in simulation.
 */
class DLL_EXPORT SimulationListener
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~SimulationListener()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief New plugin is loaded.
     *
     * @param simulation Current simulation.
     * @param name       Plugin name.
     */
    virtual void onPluginLoad(Simulation& simulation, const String& name)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
