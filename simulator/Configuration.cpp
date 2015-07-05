/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

FilePath Configuration::buildFilePath(const FilePath& filename) const NOEXCEPT
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Return file path
    return sourceFile.parent_path() / filename;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
