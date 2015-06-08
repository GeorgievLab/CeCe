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
#include <string>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief File path type.
 */
using FilePath = std::string;

/* ************************************************************************ */

/**
 * @brief Tests if file path exists.
 *
 * @param path
 *
 * @return
 */
bool DLL_EXPORT fileExists(const FilePath& path) NOEXCEPT;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
