/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/compatibility.hpp"
#include "core/UniquePtr.hpp"
#include "core/String.hpp"
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

class StreamlinesApi : public LibraryApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) NOEXCEPT override
    {
        return makeUniquePtr<Module>();
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(streamlines, StreamlinesApi)

/* ************************************************************************ */
