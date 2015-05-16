
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/Log.hpp"

// Module
#include "Module.hpp"

// Boost
#include <boost/filesystem/path.hpp>

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    try
    {
        return new module::python::Module{*simulation, boost::filesystem::path(name)};
    }
    catch (const std::exception& e)
    {
        Log::warning(e.what());
    }

    return nullptr;
}

/* ************************************************************************ */
