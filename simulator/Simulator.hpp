
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <atomic>
#include <vector>
#include <cassert>

// Simulator
#include "core/Units.hpp"
#include "simulator/Module.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Simulator class.
 *
 * Simulator handles simulation of the given (owned) world in current thread.
 */
class Simulator final
{


// Public Types
public:


    /// Module container type.
    using ModuleContainer = std::vector<std::unique_ptr<Module>>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Simulator();


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
     * @brief Returns simulation time step.
     *
     * @return
     */
    units::Duration getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief Returns current world.
     *
     * @return A pointer to current world or nullptr.
     */
    World* getWorld() const noexcept
    {
        return m_world.get();
    }


    /**
     * @brief Return a list of modules.
     *
     * @return
     */
    const ModuleContainer& getModules() const noexcept
    {
        return m_modules;
    }


// Public Mutators
public:


    /**
     * @brief Set new current world. Previous world will be deleted.
     *
     * @param world
     */
    void setWorld(std::unique_ptr<World> world) noexcept
    {
        m_world = std::move(world);
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt
     */
    void setTimeStep(units::Duration dt) noexcept
    {
        m_timeStep = dt;
    }


    /**
     * @brief Add new module.
     *
     * @param mod
     *
     * @return A pointer to inserted module.
     */
    template<typename T>
    T* addModule(std::unique_ptr<T> mod)
    {
        assert(mod);
        m_modules.push_back(std::move(mod));
        return reinterpret_cast<T*>(m_modules.back().get());
    }


    /**
     * @brief Create module.
     *
     * @param args...
     *
     * @return A pointer to created module.
     */
    template<typename T, typename... Args>
    T* createModule(Args&&... args)
    {
        return addModule(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
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
     */
    void update(units::Duration dt);


    /**
     * @brief Update simulation by time step.
     */
    void update()
    {
        update(getTimeStep());
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     *
     * @param context
     */
    void renderInit(render::Context& context);
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render simulation.
     *
     * @param context Render context.
     */
    void render(render::Context& context);
#endif


// Data Members
private:

    /// Flag if thread is running
    std::atomic<bool> m_isRunning;

    /// Simulation step.
    units::Duration m_timeStep;

    /// Simulated world.
    std::unique_ptr<World> m_world;

    /// Simulation modules.
    ModuleContainer m_modules;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
