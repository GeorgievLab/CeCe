
// Declaration
#include "Program.hpp"

// Module
#include "Python.hpp"
#include "Utils.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

namespace module {
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

void Program::operator()(simulator::Object& object, units::Duration dt) const
{
    if (!m_call)
        return;

    // Call function
    if (!call(m_call, &object, dt))
        throw Exception();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
