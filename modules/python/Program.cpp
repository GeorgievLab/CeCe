
// Declaration
#include "Program.hpp"

// Module
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

void Program::operator()(simulator::Object& object, core::units::Duration dt) const
{
    if (!m_call)
        return;

    // Call function
    if (!makeHandle(PyObject_CallFunctionObjArgs(m_call.get(), NULL)))
        throw Exception();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
