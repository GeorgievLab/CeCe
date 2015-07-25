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

#include "core/Units.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Functor for storing cell molecules amount.
 */
class StoreMolecules
{

// Public Operations
public:


    /**
     * @brief Call operator.
     *
     * @param object     Simulation object.
     * @param simulation Simulation.
     */
    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration);

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
