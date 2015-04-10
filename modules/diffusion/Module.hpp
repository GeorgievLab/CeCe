
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/GridValue.hpp"
#include "render/Grid.hpp"
#endif

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Grid type.
    using GridType = simulator::Grid<float>;

    /// Grid size type.
    using SizeType = GridType::SizeType;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module();


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
    GridType& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridType& getGrid() const noexcept
    {
        return m_grid;
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    render::GridValue& getRenderObject() noexcept
    {
        return m_renderObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    const render::GridValue& getRenderObject() const noexcept
    {
        return m_renderObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns grid render object.
     *
     * @return
     */
    render::Grid& getRenderGridObject() noexcept
    {
        return m_renderGridObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns grid render object.
     *
     * @return
     */
    const render::Grid& getRenderGridObject() const noexcept
    {
        return m_renderGridObject;
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
    void renderInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void render(render::Context& context, const simulator::World& world) override;
#endif


// Private Data Members
private:

    /// Velocity grid.
    GridType m_grid;

#ifdef ENABLE_RENDER
    /// Render grid for velocities
    render::GridValue m_renderObject;
#endif

#ifdef ENABLE_RENDER
    /// Render grid
    render::Grid m_renderGridObject;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
