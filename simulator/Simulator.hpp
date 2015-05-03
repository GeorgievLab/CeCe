
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <atomic>
#include <vector>
#include <cassert>
#include <map>

// Simulator
#include "core/Units.hpp"
#include "simulator/Module.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;
class Library;

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


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context);
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render simulation.
     *
     * @param context Render context.
     */
    void draw(render::Context& context);
#endif


    /**
     * @brief Load library into cache and return pointer.
     *
     * In case the library was loaded before, it't not loaded again.
     *
     * @param name
     *
     * @return
     */
    Library* loadLibrary(const std::string& name);


// Data Members
private:

    /// Flag if thread is running
    std::atomic<bool> m_isRunning{false};

    /// Current simulation
    std::unique_ptr<Simulation> m_simulation;

    /// Cache for loaded libraries.
    std::map<std::string, std::unique_ptr<Library>> m_libraries;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
