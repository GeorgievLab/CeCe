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
#include "core/InStream.hpp"
#include "core/FilePath.hpp"
#include "simulator/SimulationLoader.hpp"

/* ************************************************************************ */

namespace simulator { class Simulation; }

/* ************************************************************************ */

namespace loader {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML parser simulation factory.
 */
class SimulationLoader : public simulator::SimulationLoader
{


// Protected Operations
protected:


    /**
     * @brief Read simulation from input stream.
     *
     * @param is       Source stream.
     * @param filename Source file name.
     *
     * @return Created simulation.
     */
    UniquePtr<simulator::Simulation> fromStream(InStream& is,
        const FilePath& filename = "<stream>") const override;


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    void toStream(OutStream& os, const simulator::Simulation& simulation,
        const FilePath& filename = "<stream>") const override;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
