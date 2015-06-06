
/* ************************************************************************ */

// Declaration
#include "core/FilePath.hpp"

// C++
//#include <cstdio>
#include <fstream>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

bool fileExists(const FilePath& path) NOEXCEPT
{
    /*
    FILE* f = fopen(path.c_str(), "r");

    if (f)
    {
        fclose(f);
        return true;
    }
    else
    {
        return false;
    }
    */
    std::ifstream f(path.c_str());
    return f.good();
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
