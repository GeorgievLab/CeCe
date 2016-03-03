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
#include "cece/module/Module.hpp"
#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#endif

// Plugin
#include "cece/plugins/python/Handle.hpp"
#include "cece/plugins/python/Source.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Module for Python support.
 */
class Module : public module::Module
{

// Public Ctors & Dtors
public:


    using module::Module::Module;


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;

#endif


// Private Data Members
private:


    /// Source.
    Source m_source;

    /// Configure function.
    ObjectHandle m_configureFn;

    /// Update function.
    ObjectHandle m_updateFn;

#ifdef CECE_ENABLE_RENDER
    /// Draw function.
    ObjectHandle m_drawFn;
#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
