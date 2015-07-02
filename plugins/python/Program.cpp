/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "Program.hpp"

// Module
#include "Utils.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

Program::Program()
{

}

/* ************************************************************************ */

Program::~Program()
{

}

/* ************************************************************************ */

void Program::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Time dt) const
{
    if (!m_call)
        return;

    // Call function
    if (!call(m_call, &object, &simulation, dt.value()))
        throw Exception();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
