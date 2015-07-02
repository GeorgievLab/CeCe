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
 * @brief Event handler class for events in simulation.
 */
class DLL_EXPORT SimulationEventHandler
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~SimulationEventHandler()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief New plugin is loaded.
     *
     * @param name Plugin name.
     */
    virtual void onPluginLoad(const String& name)
    {
        // Nothing to do
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
