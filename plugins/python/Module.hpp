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

// This must be first
#include "Python.hpp"

// Simulator
#include "simulator/Module.hpp"
#include "render/Context.hpp"

// Module
#include "Handle.hpp"
#include "Source.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Module for Python support.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Module name. Can be path to python source.
     */
    explicit Module(const std::string& name);


// Public Operations
public:


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Configure function.
    Handle<PyObject> m_configureFn;

    /// Update function.
    Handle<PyObject> m_updateFn;

#if ENABLE_RENDER
    /// Draw function.
    Handle<PyObject> m_drawFn;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
