/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cstdint>

// Simulator
#include "core/DynamicArray.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Image data structure.
 */
struct DLL_EXPORT ImageData
{

    /// Image size.
    Size size;

    /// Image data.
    DynamicArray<std::uint8_t> data;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
