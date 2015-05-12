
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Module
#include "Module.hpp"

// Boost
#include <boost/filesystem/path.hpp>

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(simulation, name)
{
    return new module::python::Module{*simulation, boost::filesystem::path(name)};
}

/* ************************************************************************ */
