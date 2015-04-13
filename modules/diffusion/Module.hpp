
#pragma once

/* ************************************************************************ */

#ifdef THREAD_SAFE
#include <mutex>
#endif

// Simulator
#include "simulator/Module.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Grid.hpp"
#include "SignalGridDrawable.hpp"
#endif

// Module
#include "Signal.hpp"
#include "SignalGrid.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module()
        : Module(200)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param size Signal grid size.
     */
    template<typename... Args>
    Module(Args&&... args)
        : m_grid(std::forward<Args>(args)...)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    SignalGrid& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const SignalGrid& getGrid() const noexcept
    {
        return m_grid;
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    SignalGridDrawable& getDrawable() noexcept
    {
        return m_drawable;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    const SignalGridDrawable& getDrawable() const noexcept
    {
        return m_drawable;
    }
#endif

// Public Mutators
public:



// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param world World object.
     */
    void update(units::Duration dt, simulator::World& world) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::World& world) override;
#endif


// Private Data Members
private:

    /// Velocity grid.
    SignalGrid m_grid;

#ifdef ENABLE_RENDER
    /// Drawable signal grid
    SignalGridDrawable m_drawable;
#endif

#ifdef THREAD_SAFE
    std::mutex m_mutex;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
