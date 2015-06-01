
#pragma once

/* ************************************************************************ */

// Windows
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

/* ************************************************************************ */

//#if defined (_WIN32)
//	#if STATIC_BUILD
//		#define DLL_EXPORT
//    #elif defined(SIMULATOR_DLL_EXPORT)
//		#define DLL_EXPORT __declspec(dllexport)
//    #else
//        #define DLL_EXPORT __declspec(dllimport)
//    #endif
//#else
#define DLL_EXPORT
//#endif

/* ************************************************************************ */

/// Visual Studio doesn't support noexcept
#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support constexpr
#ifdef _MSC_VER
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support constexpr (fallback to const)
#ifdef _MSC_VER
#define CONSTEXPR_CONST const
#else
#define CONSTEXPR_CONST constexpr
#endif

/* ************************************************************************ */
