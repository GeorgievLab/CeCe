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
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion_python {

/* ************************************************************************ */

void init_Module(PyObject* module);

/* ************************************************************************ */

void init()
{
    PyObject* module = Py_InitModule("diffusion", nullptr);

    init_Module(module);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class DiffusionPythonApi : public plugin::Api
{

    DynamicArray<String> requiredPlugins() const override
    {
        return {"python", "diffusion"};
    }


    void initSimulation(Simulation& simulation) override
    {
        simulation.requirePlugin("python");
        plugin::diffusion_python::init();
    }
};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(diffusion_python, DiffusionPythonApi)

/* ************************************************************************ */
