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
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/StringStream.hpp"
#include "cece/simulator/Simulation.hpp"

// Diffusion
#include "cece/plugins/streamlines/Module.hpp"

// Plugin
#include "cece/plugins/python/Utils.hpp"
#include "cece/plugins/python/Type.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

/**
 * @brief Module type.
 */
class ModuleType : public Type<plugin::streamlines::Module*>
{


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    ModuleType()
        : Type("streamlines.Module")
    {
        tp_methods = m_methods;
    }


// Public Operations
public:


    /**
     * @brief Set streamlines layout.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* setLayout(SelfType* self, PyObject* args) noexcept
    {
        using namespace plugin::streamlines;

        PyObject* array;

        if (!PyArg_ParseTuple(args, "O", &array))
            return nullptr;

        // Get array iterator
        auto iter = makeHandle(PyObject_GetIter(array));

        if (!iter)
        {
            PyErr_SetString(PyExc_RuntimeError, "Not an array");
            return nullptr;
        }

        Module::Layout layout;
        StaticArray<Module::LayoutPosition, Module::LayoutPosCount> order{{
            Module::LayoutPosTop,
            Module::LayoutPosRight,
            Module::LayoutPosBottom,
            Module::LayoutPosLeft
        }};

        for (auto pos : order)
        {
            auto next = makeHandle(PyIter_Next(iter));
            if (!next)
            {
                PyErr_SetString(PyExc_RuntimeError, "Missing layout specifier(s)");
                return nullptr;
            }

            InStringStream iss(cast<String>(next));
            iss >> layout[pos];
        }

        // Set layout
        Assert(self->value);
        self->value->setLayout(layout);

        // Return None
        return none().release();
    }


    /**
     * @brief Initialize barriers.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* initBarriers(SelfType* self, PyObject* args) noexcept
    {
        PyObject* simulation;

        if (!PyArg_ParseTuple(args, "O", &simulation))
            return nullptr;

        // Find simulation type.
        auto type = findType(typeid(simulator::Simulation));

        if (PyObject_TypeCheck(simulation, type) <= 0)
        {
            PyErr_SetString(PyExc_RuntimeError, "Argument is not simulator.Simulation");
            return nullptr;
        }

        // Init barriers
        self->value->initBarriers();

        // Return None
        return none().release();
    }


    /**
     * @brief Change inlet velocity.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* setInletVelocity(SelfType* self, PyObject* args) noexcept
    {
        int position;
        float velocity;

        if (!PyArg_ParseTuple(args, "if", &position, &velocity))
            return nullptr;

        self->value->setInletVelocity(
            static_cast<plugin::streamlines::Module::LayoutPosition>(position),
            units::Velocity(velocity)
        );

        return none().release();
    }


// Private Data Members
private:

    /// Type methods
    PyMethodDef m_methods[4] = {
        {"setLayout",        (PyCFunction) setLayout,        METH_VARARGS, nullptr},
        {"initBarriers",     (PyCFunction) initBarriers,     METH_VARARGS, nullptr},
        {"setInletVelocity", (PyCFunction) setInletVelocity, METH_VARARGS, nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

ModuleType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_Module(PyObject* module)
{
    g_type.tp_base = g_type.getBaseType("simulator.Module");
    g_type.add(module);

    // Define constants
    PyModule_AddIntConstant(module, "LEFT", static_cast<int>(plugin::streamlines::Module::LayoutPosLeft));
    PyModule_AddIntConstant(module, "RIGHT", static_cast<int>(plugin::streamlines::Module::LayoutPosRight));
    PyModule_AddIntConstant(module, "TOP", static_cast<int>(plugin::streamlines::Module::LayoutPosTop));
    PyModule_AddIntConstant(module, "BOTTOM", static_cast<int>(plugin::streamlines::Module::LayoutPosBottom));
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
