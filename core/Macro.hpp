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

/**
 * @brief Convert argument to string.
 *
 * @param s Argument to convert.
 *
 * @return "s".
 */
#define STR(s) # s

/* ************************************************************************ */

/**
 * @brief Convert value of argument to string.
 *
 * @param s Macro name.
 *
 * @return Stringified macro value.
 */
#define XSTR(s) STR(s)

/* ************************************************************************ */
