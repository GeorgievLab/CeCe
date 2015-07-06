/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "core/VectorUnits.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

#ifndef __GNUC__
template class DLL_EXPORT Vector<units::Length>;
template class DLL_EXPORT Vector<units::Velocity>;
template class DLL_EXPORT Vector<units::Acceleration>;
template class DLL_EXPORT Vector<units::Force>;
#endif

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
