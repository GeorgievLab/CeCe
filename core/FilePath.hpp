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

// Deprecated std::auto_ptr
#ifndef BOOST_NO_AUTO_PTR
#define BOOST_NO_AUTO_PTR 1
#endif

// Unused warning
#ifndef BOOST_SYSTEM_NO_DEPRECATED
#define BOOST_SYSTEM_NO_DEPRECATED 1
#endif

/* ************************************************************************ */

// Boost
#include <boost/filesystem/path.hpp>

/* ************************************************************************ */

inline namespace core {

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
bool fileExists(const FilePath& path) noexcept;

/* ************************************************************************ */

}

/* ************************************************************************ */
