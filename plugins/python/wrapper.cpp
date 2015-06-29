/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "wrapper.hpp"

// C++
#include <cassert>

// Simulator
#include "core/Map.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

Map<std::type_index, PyTypeObject*> g_dynamicTypes;

/* ************************************************************************ */

}

/* ************************************************************************ */

void registerDynamic(const std::type_info& info, PyTypeObject* type)
{
    // Type is registered
    assert(g_dynamicTypes.find(std::type_index(info)) == g_dynamicTypes.end());

    g_dynamicTypes.emplace(info, type);
}

/* ************************************************************************ */

PyTypeObject* findDynamic(const std::type_info& info)
{
    auto it = g_dynamicTypes.find(std::type_index(info));
    return it != g_dynamicTypes.end() ? it->second : nullptr;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
