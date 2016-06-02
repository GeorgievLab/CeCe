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

// C++
#include <utility>

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/module/ExportModule.hpp"

/* ************************************************************************ */

namespace cece { namespace plugin { namespace diffusion { class Module; } } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Diffusion export module.
 */
class ExportModule : public module::ExportModule
{

// Public Ctors & Dtors
public:


    using module::ExportModule::ExportModule;


// Public Accessors
public:


    /**
     * @brief Returns exported signals.
     *
     * @return
     */
    const DynamicArray<String>& getSignals() const noexcept
    {
        return m_signals;
    }


    /**
     * @brief Returns if given signal is exported.
     *
     * @param name Signal name.
     *
     * @return
     */
    bool isExported(StringView name) const noexcept;


    /**
     * @brief If information about obstacles is exported.
     *
     * @return
     */
    bool isExportedObstacles() const noexcept
    {
        return m_exportObstacles;
    }


// Public Mutators
public:


    /**
     * @brief Set exported signals.
     *
     * @param signals
     */
    void setSignals(DynamicArray<String> signals) noexcept
    {
        m_signals = std::move(signals);
    }


    /**
     * @brief If information about obstacles should be exported.
     *
     * @param flag
     */
    void setExportObstacles(bool flag) noexcept
    {
        m_exportObstacles = flag;
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

    /// A pointer to diffusion module.
    ViewPtr<plugin::diffusion::Module> m_module;

    /// A list of signals to store.
    DynamicArray<String> m_signals;

    /// If information about obstacles should be exported.
    bool m_exportObstacles = false;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
