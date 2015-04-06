
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

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class StreamlinesModule : public virtual Module
{

// Public Types
public:


    /// Grid type.
    using GridType = Grid<Velocity>;

    /// Grid size type.
    using SizeType = GridType::SizeType;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    StreamlinesModule();


    /**
     * @brief Destructor.
     */
    virtual ~StreamlinesModule();


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
    void update(units::Duration dt, World& world) override;


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
    void render(render::Context& context, const World& world) override;
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

};

/* ************************************************************************ */

}

/* ************************************************************************ */
