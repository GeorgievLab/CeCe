/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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

// CeCe
#include "cece/core/Units.hpp"

// Plugin
#include "cece/plugins/streamlines/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

/**
 * @brief Module for channel streamlines.
 */
class Module : public streamlines::Module
{


// Public Accessors
public:


    /**
     * @brief Returns channel height.
     *
     * @return
     */
    units::Length getHeight() const noexcept
    {
        return m_height;
    }


// Public Mutators
public:


    /**
     * @brief Set channel height.
     *
     * @param height
     */
    void setHeight(units::Length height) noexcept
    {
        m_height = height;
    }


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     *
     * @param simulation
     */
    void init(simulator::Simulation& simulation);


    /**
     * @brief Load module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) override;


// Protected Operations
protected:


    /**
     * @brief Print streamlines informations.
     *
     * @param simulation
     */
    void printInfo(const simulator::Simulation& simulation) override;


// Private Data Members
private:

    /// Channel height.
    units::Length m_height = units::um(1);

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
