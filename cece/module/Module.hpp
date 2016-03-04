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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Atomic.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }
namespace cece { namespace config { class Configuration; } }

#ifdef CECE_ENABLE_RENDER
namespace cece { namespace render { class Context; } }
#endif

/* ************************************************************************ */

namespace cece {
namespace module {

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

#ifdef CECE_ENABLE_RENDER

    /// Z order type.
    using ZOrderType = int;

    /// Flags for module drawing.
    using DrawFlags = int;

#endif


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     */
    explicit Module(simulator::Simulation& simulation);


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Return simulation.
     *
     * @return
     */
    simulator::Simulation& getSimulation() const noexcept
    {
        return m_simulation;
    }


    /**
     * @brief Returns module priority.
     *
     * @return
     */
    PriorityType getPriority() const noexcept
    {
        return m_priority;
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns module draw Z order.
     *
     * @return
     */
    ZOrderType getZOrder() const noexcept
    {
        return m_zOrder;
    }


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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Set module draw Z order.
     *
     * @param zOrder
     */
    void setZOrder(ZOrderType zOrder) noexcept
    {
        m_zOrder = zOrder;
    }


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
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    virtual void loadConfig(const config::Configuration& config);


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    virtual void storeConfig(config::Configuration& config) const;


    /**
     * @brief Initialize module.
     *
     * @param termFlag Termination flag. If initialization is expensive it should
     *                 test this variable in case a termination request is sent.
     */
    virtual void init(AtomicBool& termFlag);


    /**
     * @brief Initialize module.
     */
    virtual void init();


    /**
     * @brief Update module state.
     */
    virtual void update();


    /**
     * @brief Terminate module.
     */
    virtual void terminate();


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param context Rendering context.
     */
    virtual void draw(render::Context& context);

#endif


// Private Data Members
private:

    /// Simulation reference.
    simulator::Simulation& m_simulation;

    /// Module update priority.
    PriorityType m_priority = 0;

#ifdef CECE_ENABLE_RENDER

    /// Module Z order.
    ZOrderType m_zOrder = 0;

    /// Module draw flags.
    DrawFlags m_drawFlags = 0;

#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
