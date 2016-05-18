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
#include "cece/config.hpp"

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER

/* ************************************************************************ */

// C++
#include <utility>

// CeCe
#include "cece/config.hpp"
#include "cece/core/String.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Visualization layer.
 */
class VisualizationLayer
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    VisualizationLayer() = default;


    /**
     * @brief Constructor.
     *
     * @param name    Layer name.
     * @param key     Key name.
     * @param enabled
     */
    explicit VisualizationLayer(String name, String key, bool enabled = false) noexcept
        : m_name(std::move(name))
        , m_key(std::move(key))
        , m_enabled(enabled)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns layer name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


    /**
     * @brief Returns enable/disable key name.
     *
     * @return
     */
    const String& getKey() const noexcept
    {
        return m_key;
    }


    /**
     * @brief If layer is enabled by default.
     *
     * @return
     */
    bool isEnabled() const noexcept
    {
        return m_enabled;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable layout.
     *
     * @return
     */
    void setEnabled(bool flag) noexcept
    {
        m_enabled = flag;
    }


// Public Operations
public:


    /**
     * @brief Toggle enable/disable.
     */
    void toggle() noexcept
    {
        m_enabled = !m_enabled;
    }


// Private Data Members
private:

    /// Layer name.
    String m_name;

    /// Layer enable/disable key.
    String m_key;

    /// If layer is enabled by default.
    bool m_enabled = false;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

#endif

/* ************************************************************************ */
