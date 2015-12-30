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

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// This must be first
#include "cece/plugins/python/Python.hpp"

// CeCe
#include "cece/core/String.hpp"
#include "cece/object/Object.hpp"
#include "cece/render/Context.hpp"

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/Source.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Python defined simulation object.
 */
class Object : public object::Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Module name. Can be path to python source.
     */
    explicit Object(simulator::Simulation& simulation, const String& name, Type type = Type::Static);


// Public Operations
public:


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    void update(units::Duration dt) override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render object.
     *
     * @param context
     */
    void draw(render::Context& context) override;

#endif


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Configure function.
    Handle<PyObject> m_configureFn;

    /// Update function.
    Handle<PyObject> m_updateFn;

#ifdef CECE_ENABLE_RENDER
    /// Draw function.
    Handle<PyObject> m_drawFn;
#endif

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
