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

// Simulator
#include "core/ViewPtr.hpp"
#include "simulator/Module.hpp"

// Plugins
#include "plugins/diffusion/Module.hpp"
#include "plugins/streamlines/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for diffusion & streamlines.
 */
class Module : public simulator::Module
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param diffusion   Diffusion module.
     * @param streamlines Streamlines module.
     */
    Module(ViewPtr<diffusion::Module> diffusion, ViewPtr<streamlines::Module> streamlines)
        : m_diffusion(diffusion)
        , m_streamlines(streamlines)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param simulation Current simulation.
     * @param dt         Simulation time step.
     */
    void update(simulator::Simulation& simulation, units::Time dt) override;


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

/* ************************************************************************ */
