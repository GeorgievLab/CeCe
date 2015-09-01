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

inline namespace core {

/* ************************************************************************ */

#if !defined(__GNUC__)
template class Vector<units::Length>;
template class Vector<units::Velocity>;
template class Vector<units::Acceleration>;
template class Vector<units::Force>;
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
