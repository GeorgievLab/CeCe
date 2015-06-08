

/* ************************************************************************ */

// Declaration
#include "core/TimeMeasurement.hpp"

// C++
#include <iostream>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

OStream* g_output = &std::cout;

/* ************************************************************************ */

OStream* getMeasureTimeOutput() NOEXCEPT
{
    return g_output;
}

/* ************************************************************************ */

void setMeasureTimeOutput(OStream* output) NOEXCEPT
{
    g_output = output;
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
