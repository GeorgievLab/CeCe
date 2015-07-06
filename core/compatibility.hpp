/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Windows
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

/* ************************************************************************ */

#if defined (_WIN32) && defined(_MSC_VER)
    #if defined(SIMULATOR_DLL_EXPORT)
        #define DLL_EXPORT __declspec(dllexport)
    #else
        #define DLL_EXPORT __declspec(dllimport)
    #endif
#else
#define DLL_EXPORT
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support noexcept
#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support constexpr until 2015
#if _MSC_VER && _MSC_VER < 1900
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support constexpr (fallback to const)
#if _MSC_VER && _MSC_VER < 1900
#define CONSTEXPR_CONST const
#else
#define CONSTEXPR_CONST constexpr
#endif

/* ************************************************************************ */
