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

FilePath Configuration::buildFilePath(const FilePath& filename) const NOEXCEPT
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Find last slash
    auto pos = sourceFile.find_last_of('/'); // TODO: modify for Windows (backslash)

    // Source file is in current directory
    if (pos == String::npos)
        return filename;

    // Get directory path without file name
    auto directory = sourceFile.substr(0, pos + 1);

    // Return file path
    return directory + filename;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
