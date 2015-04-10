
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Module.hpp"
#include "simulator/Velocity.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Circle.hpp"
#include "render/GridVector.hpp"
#endif

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Grid type.
    using GridType = simulator::Grid<simulator::Velocity>;

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


    /**
     * @brief Get radius of the main cell.
     *
     * @return
     */
    units::Length getMainCellRadius() const noexcept
    {
        return m_mainCellRadius;
    }


    /**
     * @brief Returns main cell position.
     *
     * @return
     */
    const Vector<units::Length>& getMainCellPosition() const noexcept
    {
        return m_mainCellPosition;
    }


    /**
     * @brief Returns flow speed.
     *
     * @return
     */
    float getFlowSpeed() const noexcept
    {
        return m_flowSpeed;
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    render::GridVector& getRenderObject() noexcept
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
    const render::GridVector& getRenderObject() const noexcept
    {
        return m_renderObject;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Set radius of the main cell.
     *
     * @param radius
     */
    void setMainCellRadius(units::Length radius) noexcept
    {
        m_mainCellRadius = radius;
        m_update = true;
    }


    /**
     * @brief Set main cell position.
     *
     * @param pos
     */
    void setMainCellPosition(const Vector<units::Length>& pos) noexcept
    {
        m_mainCellPosition = pos;
        m_update = true;
    }


    /**
     * @brief Set flow speed.
     *
     * @param speed.
     */
    void setFlowSpeed(float speed) noexcept
    {
        m_flowSpeed = speed;
    }


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

    /// World main cell radius.
    units::Length m_mainCellRadius = units::um(20);

    /// Main cell position.
    Vector<units::Length> m_mainCellPosition{units::um(0), units::um(0)};

    /// Flow speed.
    float m_flowSpeed = 1.f;

    /// Velocity grid.
    GridType m_grid;

    /// If streams should be updated
    bool m_update = true;

#ifdef ENABLE_RENDER
    /// Render grid for velocities
    render::GridVector m_renderObject;
#endif

#ifdef ENABLE_RENDER
    /// Render main cell.
    render::Circle m_renderCell;
#endif

#ifdef ENABLE_RENDER
    /// If render object should be updated
    bool m_renderUpdate = false;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
