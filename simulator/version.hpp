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

// Current version
//#define VERSION_MAJOR 0
//#define VERSION_MINOR 2
//#define VERSION_PATCH 0

/* ************************************************************************ */

/**
 * @brief Helper macros to convert argument to string.
 */
#define STR(s) # s
#define XSTR(s) STR(s)

/* ************************************************************************ */

/**
 * @brief Simulator special version.
 */
#ifndef VERSION_SPECIAL
#define VERSION_SPECIAL " alpha"
#endif

/* ************************************************************************ */

/**
 * @brief Simulator version macro.
 */
#define VERSION \
    XSTR(VERSION_MAJOR) "." \
    XSTR(VERSION_MINOR) "." \
    XSTR(VERSION_PATCH) \
    VERSION_SPECIAL

/* ************************************************************************ */
