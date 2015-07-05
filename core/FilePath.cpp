/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "core/FilePath.hpp"

// Boost
#include <boost/filesystem/operations.hpp>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

bool fileExists(const FilePath& path) NOEXCEPT
{
    return boost::filesystem::exists(path);
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
