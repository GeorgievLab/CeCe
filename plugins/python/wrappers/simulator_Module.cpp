/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Must be first
#include "plugins/python/Python.hpp"

// Simulator
#include "core/Assert.hpp"
#include "simulator/Module.hpp"

// Plugin
#include "plugins/python/ObjectWrapper.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Type definition.
 */
class Type : public PyTypeObject
{

// Public Types
public:


    /// Wrapper type.
    using SelfType = ObjectWrapper<simulator::Module*>;


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    explicit Type(String name)
        : PyTypeObject {PyObject_HEAD_INIT(NULL)}
        , m_name(std::move(name))
    {
        tp_name = m_name.c_str();
        tp_basicsize = sizeof(SelfType);
        tp_flags = Py_TPFLAGS_DEFAULT;

        // Type is not ready
        if (PyType_Ready(this) < 0)
            throw RuntimeException("Cannot finalize type object");
    }


// Public Operations
public:


    /**
     * @brief Finalize type definition.
     *
     * @param module
     */
    void define(View<PyObject> module) noexcept
    {
        auto type = reinterpret_cast<PyObject*>(this);

        // Find dot
        auto dot = m_name.find('.');
        Assert(dot != String::npos);

        Py_INCREF(type);
        PyModule_AddObject(module, &m_name[dot + 1], type);

        // Register dynamic type
        registerType(typeid(Type), this);
    }


// Private Data Members
private:

    /// Type name.
    String m_name;

};

/* ************************************************************************ */

Type g_type("simulator.Module");

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_simulator_Module(PyObject* module)
{
    g_type.define(module);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
