
#pragma once

/* ************************************************************************ */

// Windows
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

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
