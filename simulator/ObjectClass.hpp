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
#include "core/String.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Defines user defined object class.
 */
struct ObjectClass
{
    /// Object class name.
    String name;

    /// Basename class name.
    String baseName;

    /// Object configuration
    Configuration config;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
