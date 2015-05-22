
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <atomic>
#include <cassert>

// Simulator
#include "core/Units.hpp"

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
 * Simulator handles simulation of the given (owned) world in current thread.
 */
class Simulator final
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Simulator();


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

// Public Mutators
public:


    /**
     * @brief Change current simulation. The old simulation will be deleted.
     *
     * @param simulation New simulation.
     */
    void setSimulation(std::unique_ptr<Simulation> simulation) noexcept
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
    void stop();


    /**
     * @brief Perform one simulation step.
     */
    void step();


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Update simulation by time step.
     *
     * @param dt Time step.
     *
     * @return If next step can be calculated.
     */
    bool update(units::Duration dt);


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update();


#if ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     */
    void drawInit();
#endif


#if ENABLE_RENDER
    /**
     * @brief Render simulation.
     */
    void draw();
#endif


#if ENABLE_RENDER
    /**
     * @brief Finalize rendering.
     */
    void drawFinalize();
#endif


// Data Members
private:

    /// Flag if thread is running
    std::atomic<bool> m_isRunning{false};

    /// Current simulation
    std::unique_ptr<Simulation> m_simulation;

#if ENABLE_RENDER
    /// Rendering context.
    render::Context m_renderContext;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
