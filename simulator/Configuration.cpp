
/* ************************************************************************ */

// Declaration
#include "simulator/Configuration.hpp"

// C++
#include <cassert>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

std::string ConfigurationBase::buildFilePath(const std::string& filename) const NOEXCEPT
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Find last slash
    auto pos = sourceFile.find_last_of('/'); // TODO: modify for Windows (backslash)

    // Source file is in current directory
    if (pos == std::string::npos)
        return filename;

    // Get directory path without file name
    auto directory = sourceFile.substr(0, pos + 1);

    // Return file path
    return directory + filename;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
