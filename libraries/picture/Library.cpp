
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "simulator/Library.hpp"
#include "simulator/PluginApi.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

class PictureApi : public simulator::PluginApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        return std::unique_ptr<simulator::Module>(new module::picture::Module{});
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(picture, PictureApi)

/* ************************************************************************ */
