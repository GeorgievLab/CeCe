/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Python requires to be included first because it sets some parameters to stdlib
#include "Python.hpp"

/* ************************************************************************ */

// Declaration
#include "Initializer.hpp"

// Module
#include "Utils.hpp"
#include "Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

Initializer::Initializer()
{

}

/* ************************************************************************ */

Initializer::~Initializer()
{

}

/* ************************************************************************ */

void Initializer::operator()(simulator::Simulation& simulation) const
{
    if (!m_call)
        return;

    // Call function
    if (!call(m_call, &simulation))
        throw Exception();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
