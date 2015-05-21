
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/Log.hpp"

// Simulator
#include "simulator/Library.hpp"

// Module
#include "Module.hpp"

// Boost
#include <boost/filesystem/path.hpp>

/* ************************************************************************ */

class PythonApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        try
        {
            return std::unique_ptr<simulator::Module>(new module::python::Module{simulation, boost::filesystem::path(name)});
        }
        catch (const std::exception& e)
        {
            Log::warning(e.what());
        }

        return nullptr;
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_IMPL(PythonApi)

/* ************************************************************************ */
