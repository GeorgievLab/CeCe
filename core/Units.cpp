/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "core/Units.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

#if !_MSC_VER && 0
template class Unit<List<BaseLength>, List<>>;
template class Unit<List<BaseMass>, List<>>;
template class Unit<List<BaseTime>, List<>>;
template class Unit<List<BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength>, List<BaseTime>>;
template class Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
template class Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
