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

/* ************************************************************************ */

#ifdef ENABLE_RENDER
namespace render { class Context; }
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Abstract class for simulation modules.
 *
 * Module is a part of simulation that is called once per simulation iteration.
 * Their main purpose is act as global object.
 *
 * Module live looks like:
 * 1) construct
 * 3) [optional] `loadConfig` (multiple times)
 * 2) `init`
 * 4) `update`
 * 5) [optional] `draw`
 * 6) goto 4 unless simulation ends
 * 7) `terminate`
 * 8) destructor
 *
 * `loadConfig` can be called multiple times when something needs to override some
 * module configuration. `storeConfig` can be called anytime it's main purpose is
 * store current module configuration - module configuration is modified during
 * runtime and something wants to store for further use.
 *
 * Module priority main purpose is for ordering module update execution. Some
 * modules can be dependent on other modules and they may need their result in
 * own update (result of other module can be deleted at the end of iteration).
 * Bigger value means the module has bigger priority and it's update is called
 * foremost.
 *
 * Z-order is similar to priority but for rendering.
 */
class Module
{

// Public Types
public:


    /// Module update priority type.
    using PriorityType = int;


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


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns module priority.
     *
     * @return
     */
    PriorityType getPriority() const noexcept
    {
        return m_priority;
    }


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


    /**
     * @brief Set module priority.
     *
     * @param priority
     */
    void setPriority(PriorityType priority) noexcept
    {
        m_priority = priority;
    }


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
     * @brief Initialize module.
     *
     * This function is called before the simulation is started. Allows to module
     * prepare internal data that is dependent on current simulation. For data
     * independent on current simulation, use constructor instead.
     *
     * @param simulation Current simulation.
     */
    virtual void init(Simulation& simulation);


    /**
     * @brief Terminate module.
     *
     * This function is called after the simulation is finished. Allows to module
     * cleanup internal data that is dependent on current simulation. For data
     * independent on current simulation, use destructor instead.
     *
     * @param simulation Current simulation.
     */
    virtual void terminate(Simulation& simulation);


    /**
     * @brief Update module state.
     *
     * @param simulation Simulation object.
     * @param dt         Simulation time step.
     */
    virtual void update(Simulation& simulation, units::Time dt);


    /**
     * @brief Load module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Source configuration.
     */
    virtual void loadConfig(Simulation& simulation, const Configuration& config);


    /**
     * @brief Store module configuration.
     *
     * @param simulation Current simulation.
     * @param config     Output configuration.
     */
    virtual void storeConfig(Simulation& simulation, Configuration& config);


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param simulation Current simulation.
     * @param context    Rendering context.
     */
    virtual void draw(const Simulation& simulation, render::Context& context);
#endif


// Private Data Members
private:

    /// Module update priority.
    PriorityType m_priority = 0;

#if ENABLE_RENDER
    /// Module Z order.
    ZOrderType m_zOrder = 0;
#endif

#if ENABLE_RENDER
    /// Module draw flags.
    DrawFlags m_drawFlags = 0;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
