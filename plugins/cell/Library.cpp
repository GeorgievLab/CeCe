
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cassert>
#include <cstring>

// Simulator
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Library.hpp"
#include "simulator/PluginApi.hpp"

// Module
#include "Generator.hpp"
#include "Yeast.hpp"
#include "Cell.hpp"

/* ************************************************************************ */

using namespace simulator;
using namespace plugin::cell;

/* ************************************************************************ */

class CellApi : public PluginApi
{
    std::unique_ptr<Module> createModule(Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        if (name == "generator")
            return std::unique_ptr<Module>(new Generator{});

        return nullptr;
    }

    std::unique_ptr<Object> createObject(Simulation& simulation, const std::string& name, bool dynamic = true) NOEXCEPT override
    {
        auto type = dynamic ? Object::Type::Dynamic : Object::Type::Static;

        if (name == "Yeast")
        {
            return std::unique_ptr<Object>(new Yeast{simulation, type});
        }
        else if (name == "Cell")
        {
            return std::unique_ptr<Object>(new Cell{simulation, type});
        }

        return nullptr;
    }

};

/* ************************************************************************ */

DEFINE_LIBRARY(cell, CellApi)

/* ************************************************************************ */
