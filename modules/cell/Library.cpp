
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
#include "simulator/LibraryApi.hpp"

// Module
#include "Generator.hpp"
#include "Yeast.hpp"
#include "Cell.hpp"

/* ************************************************************************ */

class CellApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        if (name == "generator")
            return std::unique_ptr<simulator::Module>(new module::cell::Generator{});

        return nullptr;
    }

    std::unique_ptr<simulator::Object> createObject(simulator::Simulation& simulation, const std::string& name, bool dynamic = true) NOEXCEPT override
    {
        auto type = dynamic ? simulator::Object::Type::Dynamic : simulator::Object::Type::Static;

        if (name == "Yeast")
        {
            return std::unique_ptr<simulator::Object>(new module::cell::Yeast{simulation, type});
        }
        else if (name == "Cell")
        {
            return std::unique_ptr<simulator::Object>(new module::cell::Cell{simulation, type});
        }

        return nullptr;
    }

};

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_IMPL(cell, CellApi)

/* ************************************************************************ */
