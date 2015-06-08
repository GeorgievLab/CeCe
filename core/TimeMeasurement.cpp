/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
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
