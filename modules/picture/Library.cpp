
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "simulator/Library.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

class PictureApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        return std::unique_ptr<simulator::Module>(new module::picture::Module{});
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_IMPL(PictureApi)

/* ************************************************************************ */
