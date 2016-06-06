/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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


// Private Data Members
private:

    /// Type methods
    PyMethodDef m_methods[1] = {
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
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
