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

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/module/Module.hpp"

// Plugins
#include "cece/plugins/diffusion/Module.hpp"
#include "cece/plugins/streamlines/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for diffusion & streamlines.
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


// Private Data Members
private:

    /// Diffusion module.
    ViewPtr<diffusion::Module> m_diffusion;

    /// Streamlines module.
    ViewPtr<streamlines::Module> m_streamlines;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
