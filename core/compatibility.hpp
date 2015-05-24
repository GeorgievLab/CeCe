
#pragma once

/* ************************************************************************ */

// Windows
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support NOEXCEPT
#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT NOEXCEPT
#endif

/* ************************************************************************ */

/// Visual Studio doesn't support CONSTEXPR
#ifdef _MSC_VER
#define CONSTEXPR
#else
#define CONSTEXPR CONSTEXPR
#endif

/* ************************************************************************ */
