/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Boost
#include <boost/filesystem/path.hpp>

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
using FilePath = boost::filesystem::path;

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
