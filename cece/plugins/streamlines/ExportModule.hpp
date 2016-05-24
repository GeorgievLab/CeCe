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
#include "cece/module/ExportModule.hpp"

/* ************************************************************************ */

namespace cece { namespace plugin { namespace streamlines { class Module; } } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Streamlines export module.
 */
class ExportModule : public module::ExportModule
{

// Public Ctors & Dtors
public:


    using module::ExportModule::ExportModule;


// Public Accessors
public:


    /**
     * @brief If density is exported.
     *
     * @return
     */
    bool isDensityExported() const noexcept
    {
        return m_density;
    }


    /**
     * @brief If node populations are exported.
     *
     * @return
     */
    bool isPopulationsExported() const noexcept
    {
        return m_populations;
    }


// Public Mutators
public:


    /**
     * @brief Set if density is exported.
     *
     * @param flag
     */
    void setDensityExported(bool flag) noexcept
    {
        m_density = flag;
    }


    /**
     * @brief Set if node populations are exported.
     *
     * @param flag
     */
    void setPopulationsExported(bool flag) noexcept
    {
        m_populations = flag;
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
     * @param config Destination configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Initialize module.
     */
    void init() override;


    /**
     * @brief Update module state.
     */
    void update() override;


// Private Data Members
private:

    /// A pointer to streamlines module.
    ViewPtr<plugin::streamlines::Module> m_module;

    /// If cell density should be exported.
    bool m_density = false;

    /// If node populations should be exported.
    bool m_populations = false;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
