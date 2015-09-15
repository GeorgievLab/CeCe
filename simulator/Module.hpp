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
#include "core/Units.hpp"
#include "simulator/Configuration.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Abstract class for simulation modules.
 */
class Module
{

// Public Types
public:


#if ENABLE_RENDER
    /// Z order type.
    using ZOrderType = int;
#endif


#if ENABLE_RENDER
    /// Flags for module drawing.
    using DrawFlags = int;
#endif


// Public Ctors & Dtors
public:


#if ENABLE_RENDER
    /**
     * @brief Constructor.
     *
     * @param zOrder Module draw Z order.
     */
    explicit Module(ZOrderType zOrder = 0) noexcept
        : m_zOrder(zOrder)
    {
        // Nothing to do
    }
#endif


    /**
     * @brief Destructor.
     */
    virtual ~Module()
    {
        // Nothing to do
    }


// Public Accessors
public:


#if ENABLE_RENDER
    /**
     * @brief Returns module draw Z order.
     *
     * @return
     */
    ZOrderType getZOrder() const noexcept
    {
        return m_zOrder;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Returns module draw flags.
     *
     * @return
     */
    DrawFlags getDrawFlags() const noexcept
    {
        return m_drawFlags;
    }
#endif


// Public Mutators
public:


#if ENABLE_RENDER
    /**
     * @brief Set module draw Z order.
     *
     * @param zOrder
     */
    void setZOrder(ZOrderType zOrder) noexcept
    {
        m_zOrder = zOrder;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Set module draw flags.
     *
     * @param flags
     */
    void setDrawFlags(DrawFlags flags) noexcept
    {
        m_drawFlags = flags;
    }
#endif


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation Simulation object.
     */
    virtual void update(units::Duration dt, Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    virtual void configure(const Configuration& config, Simulation& simulation)
    {
#if ENABLE_RENDER
        setZOrder(config.get("z-order", getZOrder()));
#endif
    }


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    virtual void draw(render::Context& context, const Simulation& simulation)
    {
        // Nothing to do
    }
#endif


// Private Data Members
private:

#if ENABLE_RENDER
    /// Module Z order.
    ZOrderType m_zOrder;
#endif

#if ENABLE_RENDER
    /// Module draw flags.
    DrawFlags m_drawFlags = 0;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
