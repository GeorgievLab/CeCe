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

template class DLL_EXPORT Unit<List<BaseLength>, List<>>;
template class DLL_EXPORT Unit<List<BaseMass>, List<>>;
template class DLL_EXPORT Unit<List<BaseTime>, List<>>;
template class DLL_EXPORT Unit<List<BaseLength, BaseLength>, List<>>;
template class DLL_EXPORT Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
template class DLL_EXPORT Unit<List<BaseLength>, List<BaseTime>>;
template class DLL_EXPORT Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
template class DLL_EXPORT Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
template class DLL_EXPORT Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
template class DLL_EXPORT Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
template class DLL_EXPORT Unit<List<BaseLength, BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
