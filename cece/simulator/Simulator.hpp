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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// C++
#include <atomic>

// CeCe
#include "cece/export.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UniquePtr.hpp"
#ifdef CECE_ENABLE_RENDER
#include "cece/render/Context.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Simulator class.
 *
 * Simulator handles simulation of the given (owned) simulation in current thread.
 */
class CECE_EXPORT Simulator final
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Simulator()
    {
        stop();
    }


/// Public Accessors
public:


    /**
     * @brief If simulation is running.
     *
     * @return
     */
    bool isRunning() const noexcept
    {
        return m_isRunning;
    }


    /**
     * @brief Returns current simulation.
     *
     * @return A pointer to current simulation or nullptr.
     */
    Simulation* getSimulation() const noexcept
    {
        return m_simulation.get();
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns rendering context.
     *
     * @return
     */
    render::Context& getRenderContext() noexcept
    {
        return m_renderContext;
    }


    /**
     * @brief Returns rendering context.
     *
     * @return
     */
    const render::Context& getRenderContext() const noexcept
    {
        return m_renderContext;
    }


    /**
     * @brief Returns if rendering context is initialized.
     *
     * @return
     */
    bool isDrawInitialized() noexcept
    {
        return m_renderContext.isInitialized();
    }

#endif


// Public Mutators
public:


    /**
     * @brief Change current simulation. The old simulation will be deleted.
     *
     * @param simulation New simulation.
     */
    void setSimulation(UniquePtr<Simulation> simulation) noexcept
    {
        m_simulation = std::move(simulation);
    }


// Public Operations
public:


    /**
     * @brief Start simulation.
     */
    void start();


    /**
     * @brief Stop simulation.
     */
    void stop()
    {
        m_isRunning = false;
    }


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update();


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Initialize simulation for rendering.
     *
     * @param clearColor
     */
    void drawInit(const render::Color& clearColor = render::colors::WHITE)
    {
        m_renderContext.init(clearColor);
    }


    /**
     * @brief Render simulation.
     *
     * @param width
     * @param height
     */
    void draw(unsigned int width, unsigned int height);

#endif


// Data Members
private:

    /// Flag if thread is running
    std::atomic<bool> m_isRunning{false};

#ifdef CECE_ENABLE_RENDER
    /// Rendering context.
    render::Context m_renderContext;
#endif

    /// Current simulation
    UniquePtr<Simulation> m_simulation;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
