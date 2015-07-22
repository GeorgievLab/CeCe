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

// C++
#include <mutex>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Mutex class
 */
using Mutex = std::mutex;

/* ************************************************************************ */

/**
 * @brief Mutex guard.
 */
using MutexGuard = std::lock_guard<Mutex>;

/* ************************************************************************ */

}

/* ************************************************************************ */
