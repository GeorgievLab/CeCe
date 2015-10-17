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

// Cell
#include "plugins/cell/CellBase.hpp"

// Plugin
#include "plugins/python/Type.hpp"
#include "plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell_python {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

using namespace plugin::python;

/* ************************************************************************ */

/**
 * @brief Basic cell type.
 */
class CellBaseType : public Type<plugin::cell::CellBase*>
{


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    CellBaseType()
        : Type("cell.CellBase")
    {
        tp_base = getBaseType("simulator.Object");
        tp_getset = m_properties;
        tp_methods = m_methods;
    }


// Public Operations
public:


    /**
     * @brief Returns cell volume.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getVolume(SelfType* self) noexcept
    {
        return makeObject(self->value->getVolume()).release();
    }


    /**
     * @brief Set cell volume.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setVolume(SelfType* self, PyObject* value) noexcept
    {
        self->value->setVolume(cast<units::Volume>(value));

        return 0;
    }


    /**
     * @brief Returns cell growth rate.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getGrowthRate(SelfType* self) noexcept
    {
        return makeObject(self->value->getGrowthRate()).release();
    }


    /**
     * @brief Set cell growth rate.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setGrowthRate(SelfType* self, PyObject* value) noexcept
    {
        self->value->setGrowthRate(cast<plugin::cell::CellBase::GrowthRate>(value));

        return 0;
    }


    /**
     * @brief Returns required molecule count.
     *
     * @param self
     * @param args
     *
     * @return
     */
    static PyObject* getMoleculeCount(SelfType* self, PyObject* args) noexcept
    {
        char* name;

        if (!PyArg_ParseTuple(args, "s", &name))
            return nullptr;

        return makeObject(self->value->getMoleculeCount(name)).release();
    }


    /**
     * @brief Kill the cell.
     *
     * @param self
     *
     * @return
     */
    static PyObject* kill(SelfType* self) noexcept
    {
        self->value->kill();

        return none().release();
    }


// Private Data Members
private:

    /// Type properties.
    PyGetSetDef m_properties[3] = {
        {const_cast<char*>("volume"),     (getter) getVolume,     (setter) setVolume,       nullptr},
        {const_cast<char*>("growthRate"), (getter) getGrowthRate, (setter) setGrowthRate,   nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Type methods.
    PyMethodDef m_methods[3] = {
        {"moleculeCount", (PyCFunction) getMoleculeCount, METH_VARARGS, nullptr},
        {"kill",          (PyCFunction) kill,             METH_NOARGS,  nullptr},
        {nullptr}  /* Sentinel */
    };

};

/* ************************************************************************ */

CellBaseType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_CellBase(PyObject* module)
{
    g_type.add(module);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
