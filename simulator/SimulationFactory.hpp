/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Simulation factory interface.
 */
class DLL_EXPORT SimulationFactory
{

// Public Dtors
public:


    /**
     * @brief Virtual destructor.
     */
    virtual ~SimulationFactory()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Create empty simulation.
     *
     * @return
     */
    virtual UniquePtr<Simulation> createSimulation() const;


    /**
     * @brief Create a new simulation from source file.
     *
     * @param filename
     *
     * @return
     */
    virtual UniquePtr<Simulation> fromFile(const FilePath& filename) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param source
     *
     * @return
     */
    virtual UniquePtr<Simulation> fromSource(const String& source, const FilePath& filename = "<source>") const = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
