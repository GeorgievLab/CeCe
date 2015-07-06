/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
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
 * @brief String type.
 */
using String = std::string;

/* ************************************************************************ */

#if _MSC_VER
DLL_EXPORT_EXTERN template class DLL_EXPORT std::allocator<char>;
DLL_EXPORT_EXTERN template class DLL_EXPORT std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
#endif

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
