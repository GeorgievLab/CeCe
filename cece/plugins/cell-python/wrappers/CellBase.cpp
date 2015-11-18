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

// Cell
#include "cece/plugins/cell/CellBase.hpp"

// Plugin
#include "cece/plugins/python/Type.hpp"
#include "cece/plugins/python/Utils.hpp"

/* ************************************************************************ */

namespace cece {
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
        tp_getset = m_properties;
        tp_methods = m_methods;
        tp_as_mapping = &m_map;
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
     * @brief Returns cell max volume.
     *
     * @param self
     *
     * @return
     */
    static PyObject* getVolumeMax(SelfType* self) noexcept
    {
        return makeObject(self->value->getVolumeMax()).release();
    }


    /**
     * @brief Set cell max volume.
     *
     * @param self
     * @param value
     *
     * @return
     */
    static int setVolumeMax(SelfType* self, PyObject* value) noexcept
    {
        self->value->setVolumeMax(cast<units::Volume>(value));

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


    /**
     * @brief Returns a number of distinct molecules.
     *
     * @param self
     *
     * @return
     */
    static Py_ssize_t getMappingSize(SelfType* self) noexcept
    {
        return self->value->getMolecules().size();
    }


    /**
     * @brief Returns number of molecules.
     *
     * @param self
     * @param key
     *
     * @return
     */
    static PyObject* getMappingSubscript(SelfType* self, PyObject* key) noexcept
    {
        const char* name = PyString_AsString(key);

        if (!name)
            return nullptr;

        return makeObject(self->value->getMoleculeCount(name)).release();
    }


    /**
     * @brief Set number of molecules.
     *
     * @param self
     * @param name
     * @param value
     *
     * @return
     */
    static int setMappingSubscript(SelfType* self, PyObject* key, PyObject* value) noexcept
    {
        const char* name = PyString_AsString(key);

        if (!name)
            return 1;

        auto amount = PyLong_AsLong(value);

        if (amount == -1)
            return 1;

        // Change molecule count
        self->value->setMoleculeCount(name, amount);

        return 0;
    }


// Private Data Members
private:

    /// Type properties.
    PyGetSetDef m_properties[4] = {
        {const_cast<char*>("volume"),     (getter) getVolume,     (setter) setVolume,       nullptr},
        {const_cast<char*>("volumeMax"),  (getter) getVolumeMax,  (setter) setVolumeMax,    nullptr},
        {const_cast<char*>("growthRate"), (getter) getGrowthRate, (setter) setGrowthRate,   nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Type methods.
    PyMethodDef m_methods[3] = {
        {"moleculeCount", (PyCFunction) getMoleculeCount, METH_VARARGS, nullptr},
        {"kill",          (PyCFunction) kill,             METH_NOARGS,  nullptr},
        {nullptr}  /* Sentinel */
    };

    /// Mapping methods.
    PyMappingMethods m_map = {
        (lenfunc) getMappingSize,
        (binaryfunc) getMappingSubscript,
        (objobjargproc) setMappingSubscript
    };
};

/* ************************************************************************ */

CellBaseType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_CellBase(PyObject* module)
{
    // Set parent type.
    g_type.tp_base = g_type.getBaseType("simulator.Object");
    g_type.add(module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
