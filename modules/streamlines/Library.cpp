
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

class StreamlinesApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        return std::unique_ptr<simulator::Module>(new module::streamlines::Module{});
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_IMPL(streamlines, StreamlinesApi)

/* ************************************************************************ */
