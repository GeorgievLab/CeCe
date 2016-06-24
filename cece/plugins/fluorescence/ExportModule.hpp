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
#include "cece/core/VectorUnits.hpp"
#include "cece/module/ExportModule.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace fluorescence {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Fluorescence export module.
 */
class ExportModule : public module::ExportModule
{

// Public Ctors & Dtors
public:


    using module::ExportModule::ExportModule;


// Public Accessors
public:


    /**
     * @brief Returns measurement area center position.
     *
     * @return
     */
    const units::PositionVector& getPosition() const noexcept
    {
        return m_position;
    }


    /**
     * @brief Returns measurement area size.
     *
     * @return
     */
    const units::SizeVector& getSize() const noexcept
    {
        return m_size;
    }


// Public Mutators
public:


    /**
     * @brief Set measurement area center position.
     *
     * @param position New position.
     */
    void setPosition(units::PositionVector position) noexcept
    {
        m_position = position;
    }


    /**
     * @brief Set measurement area size.
     *
     * @param size New size.
     */
    void setSize(units::SizeVector size) noexcept
    {
        m_size = size;
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

    /// Fluorescence module.
    ViewPtr<fluorescence::Module> m_module;

    /// Measurement area center position.
    units::PositionVector m_position = Zero;

    /// Measurement area size.
    units::SizeVector m_size = Zero;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
