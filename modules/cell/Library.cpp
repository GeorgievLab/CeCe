
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cassert>
#include <cstring>

// Simulator
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Generator.hpp"
#include "Yeast.hpp"
#include "Cell.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_INIT(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

DEFINE_LIBRARY_FINALIZE(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_MODULE(simulation, name)
{
    if (!strcmp(name, "generator"))
        return new module::cell::Generator{};

    return nullptr;
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_OBJECT(simulation, name, flags)
{
    std::string name_str(name);
    auto type = (flags == 0) ? simulator::Object::Type::Static : simulator::Object::Type::Dynamic;

    if (name_str == "Yeast")
    {
        return new module::cell::Yeast{*simulation, type};
    }
    else if (name_str == "Cell")
    {
        return new module::cell::Cell{*simulation, type};
    }

    return nullptr;
}

/* ************************************************************************ */
