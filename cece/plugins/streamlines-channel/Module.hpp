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
 *
 * The main difference between standard streamlines module is support of
 * channel height that affects velocity profiles. This is done by using
 * modified D2Q9 model.
 */
class Module : public streamlines::Module
{

// Public Ctors & Dtors
public:


    using streamlines::Module::Module;


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
     * @brief Initialize module.
     *
     * @param termFlag Termination flag.
     */
    void init(AtomicBool& termFlag) override;


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


// Protected Operations
protected:


    /**
     * @brief Create fluid dynamics.
     *
     * @return
     */
    UniquePtr<streamlines::Dynamics> createFluidDynamics() const override;


    /**
     * @brief Create border dynamics.
     *
     * @param pos
     *
     * @return
     */
    UniquePtr<streamlines::Dynamics> createBorderDynamics(LayoutPosition pos) const override;


    /**
     * @brief Print streamlines informations.
     */
    void printInfo() override;


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
