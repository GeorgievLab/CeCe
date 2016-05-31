/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/simulator/IterationType.hpp"

// Plugins
#include "cece/plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }
namespace cece { namespace plugin { namespace streamlines { class Module; } } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for diffusion & streamlines.
 */
class Module : public diffusion::Module
{

// Public Ctors & Dtors
public:


    using diffusion::Module::Module;


// Public Accessors
public:


    /**
     * @brief Returns a number of inner iterations.
     *
     * @return
     */
    simulator::IterationNumber getInnerIterations() const noexcept
    {
        return m_innerIterations;
    }


// Public Mutators
public:


    /**
     * @brief Set a number of inner iterations.
     *
     * @param iterations
     */
    void setInnerIterations(simulator::IterationNumber iterations) noexcept
    {
        m_innerIterations = iterations;
    }


// Public Operations
public:


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


    /**
     * @brief Initialize module.
     */
    void init() override;


// Protected Operations
protected:


    /**
     * @brief Update signal.
     *
     * @param id Signal identifier.
     */
    void updateSignal(SignalId id) override;


    /**
     * @brief Update obstacle map.
     */
    void updateObstacles() override;


// Private Data Members
private:

    /// Streamlines module.
    ViewPtr<streamlines::Module> m_streamlines;

    /// Number of inner iterations.
    simulator::IterationNumber m_innerIterations = 1;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
