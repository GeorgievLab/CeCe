/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <atomic>

// Simulator
#include "core/Units.hpp"
#include "core/UniquePtr.hpp"
#if ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Simulator class.
 *
 * Simulator handles simulation of the given (owned) simulation in current thread.
 */
class Simulator final
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


#if ENABLE_RENDER
    /**
     * @brief Returns rendering context.
     *
     * @return
     */
    render::Context& getRenderContext() noexcept
    {
        return m_renderContext;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Returns rendering context.
     *
     * @return
     */
    const render::Context& getRenderContext() const noexcept
    {
        return m_renderContext;
    }
#endif


#if ENABLE_RENDER
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
     * @brief Perform one simulation step.
     */
    void step();


    /**
     * @brief Update simulation by time step.
     *
     * @param dt Time step.
     *
     * @return If next step can be calculated.
     */
    bool update(units::Time dt);


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update();


#if ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     *
     * @param clearColor
     */
    void drawInit(const render::Color& clearColor = render::colors::WHITE)
    {
        m_renderContext.init(clearColor);
    }
#endif


#if ENABLE_RENDER
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

#if ENABLE_RENDER
    /// Rendering context.
    render::Context m_renderContext;
#endif

    /// Current simulation
    UniquePtr<Simulation> m_simulation;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
