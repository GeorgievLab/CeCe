/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

FilePath Configuration::buildFilePath(const FilePath& filename) const noexcept
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Return file path
    return sourceFile.parent_path() / filename;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
