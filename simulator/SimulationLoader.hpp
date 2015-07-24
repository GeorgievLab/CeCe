/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"
#include "core/InStream.hpp"
#include "core/OutStream.hpp"
#include "core/Exception.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Loader exception.
 */
using LoaderException = RuntimeException;

/* ************************************************************************ */

/**
 * @brief Simulation loader interface.
 */
class SimulationLoader
{

// Public Dtors
public:


    /**
     * @brief Virtual destructor.
     */
    virtual ~SimulationLoader()
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Create a new simulation from source file.
     *
     * @param filename Path to source file.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<Simulation> fromFile(const FilePath& filename) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param source   String with source.
     * @param filename Path to source file.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<Simulation> fromSource(const String& source, const FilePath& filename = "<source>") const;


    /**
     * @brief Store simulation into file.
     *
     * @param simulation Source simulation.
     * @param filename   Path to source file.
     */
    virtual void toFile(const Simulation& simulation, const FilePath& filename) const;


    /**
     * @brief Convert simulation into source code.
     *
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    virtual String toSource(const Simulation& simulation, const FilePath& filename = "<source>") const;


    /**
     * @brief Read simulation from input stream.
     *
     * @param is       Source stream.
     * @param filename Source file name.
     *
     * @return Created simulation.
     */
    virtual UniquePtr<simulator::Simulation> fromStream(InStream& is,
        const FilePath& filename = "<stream>") const
    {
        throw RuntimeException("Please implement this");
    }


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    virtual void toStream(OutStream& os, const Simulation& simulation, const FilePath& filename = "<stream>") const
    {
        throw RuntimeException("Please implement this");
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
