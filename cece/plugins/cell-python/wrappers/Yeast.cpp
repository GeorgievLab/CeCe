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
#include "cece/plugins/cell/Yeast.hpp"

// Plugin
#include "cece/plugins/python/Type.hpp"

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
 * @brief Yeast type.
 */
class YeastType : public Type<plugin::cell::Yeast*>
{


// Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    YeastType()
        : Type("cell.Yeast")
    {
        // Nothing to do
    }

};

/* ************************************************************************ */

YeastType g_type;

/* ************************************************************************ */

}

/* ************************************************************************ */

void init_Yeast(PyObject* module)
{
    g_type.tp_base = g_type.getBaseType("cell.CellBase");
    g_type.add(module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
