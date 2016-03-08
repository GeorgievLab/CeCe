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

// This must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/Exception.hpp"
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"
#include "cece/plugin/Context.hpp"

// Plugin
#include "cece/plugins/python/Module.hpp"
#include "cece/plugins/python/Object.hpp"
#include "cece/plugins/python/Program.hpp"
#include "cece/plugins/python/Initializer.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

void init_stdout(void);
void init_core(void);
void init_simulator(void);

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

/**
 * @brief Python modules initialization table.
 */
static const struct _inittab INIT_TABLE[] = {
    {const_cast<char*>("cppout"),    cece::plugin::python::init_stdout},
    {const_cast<char*>("core"),      cece::plugin::python::init_core},
    {const_cast<char*>("simulator"), cece::plugin::python::init_simulator},
    {NULL, NULL}
};

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

class PythonApi : public plugin::Api
{

    /**
     * @brief On plugin load.
     *
     * @param context Plugin context.
     */
    void onLoad(plugin::Context& context) override
    {
        if (PyImport_ExtendInittab(const_cast<struct _inittab*>(INIT_TABLE)) != 0)
            throw RuntimeException("Unable to initialize Python import table");

        // Initialize Python interpreter
        Py_Initialize();

        context.registerInitializer<plugin::python::Initializer>("python");
        context.registerModule<plugin::python::Module>("python");
        context.registerProgram<plugin::python::Program>("python");
    }


    /**
     * @brief On plugin unload.
     *
     * @param context Plugin context.
     */
    void onUnload(plugin::Context& context) override
    {
        context.unregisterModule("python");
        context.unregisterProgram("python");
        context.unregisterInitializer("python");

        Py_Finalize();
    }

};

/* ************************************************************************ */

CECE_DEFINE_PLUGIN(python, PythonApi)

/* ************************************************************************ */
