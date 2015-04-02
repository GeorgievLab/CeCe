
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>
#include <type_traits>

// Simulator
#include "simulator/Velocity.hpp"
#include "simulator/Object.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#include "render/Grid.hpp"
#include "render/GridVector.hpp"
#include "render/GridValue.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Type for step number.
 */
using StepNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief World for cells.
 */
class World
{

// Public Enums
public:

#ifdef ENABLE_RENDER

    enum RenderFlags
    {
        RENDER_NONE = 0,
        RENDER_GRID = 1,
        RENDER_VELOCITY = 2,
    };

    using RenderFlagsType = typename std::underlying_type<RenderFlags>::type;

#endif

// Public Types
public:


    /// Grid for storing velocities.
    using GridVelocity = Grid<Velocity>;

    /// Grid for storing signals.
    using GridSignal = Grid<float>;


// Public Types
public:


    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    World() noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~World();


// Public Accessors
public:


    /**
     * @brief Returns step number.
     *
     * @return
     */
    StepNumber getStepNumber() const noexcept
    {
        return m_stepNumber;
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    const ObjectContainer& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const Vector<units::Length>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Returns world width.
     *
     * @return
     */
    units::Length getWidth() const noexcept
    {
        return getSize().width;
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    units::Length getHeight() const noexcept
    {
        return getSize().height;
    }


    /**
     * @brief Returns grid with velocities.
     *
     * @return
     */
    GridVelocity& getVelocityGrid() noexcept
    {
        return m_velocityGrid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridVelocity& getVelocityGrid() const noexcept
    {
        return m_velocityGrid;
    }


    /**
     * @brief Returns grid with velocities.
     *
     * @return
     */
    GridSignal& getSignalGrid() noexcept
    {
        return m_signalGrid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridSignal& getSignalGrid() const noexcept
    {
        return m_signalGrid;
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
     * @brief Resize world.
     *
     * @param size
     */
    void resize(Vector<units::Length> size) noexcept
    {
        m_size = std::move(size);
    }


    /**
     * @brief Resize world.
     *
     * @param width
     * @param height
     */
    void resize(units::Length width, units::Length height) noexcept
    {
        resize({width, height});
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    T* addObject(std::unique_ptr<T> obj)
    {
        assert(obj);
        m_objects.push_back(std::move(obj));
        return static_cast<T*>(m_objects.back().get());
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        return addObject(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
    }


    /**
     * @brief Set radius of the main cell.
     *
     * @param radius
     */
    void setMainCellRadius(units::Length radius) noexcept
    {
        m_mainCellRadius = radius;
    }


    /**
     * @brief Set main cell position.
     *
     * @param pos
     */
    void setMainCellPosition(const Vector<units::Length>& pos) noexcept
    {
        m_mainCellPosition = pos;
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
     * @brief Clean world data.
     */
    void clean();


    /**
     * @brief Reset world.
     */
    void reset();


    /**
     * @brief Update world.
     */
    void update() noexcept;


    /**
     * @brief Recalculate flow streams.
     */
    void recalcFlowstreams();


    /**
     * @brief Recalculate diffusion.
     */
    void recalcDiffusion(units::Duration dt);


#ifdef ENABLE_RENDER

    /**
     * @brief Render world.
     *
     * @param context
     * @param flags
     */
    void render(render::Context& context, RenderFlagsType flags = RENDER_NONE);

#endif


    /**
     * @brief Calculate real world position based on grid coordinates.
     *
     * @param grid
     * @param pos
     *
     * @return
     */
    template<typename T>
    Vector<units::Length> getPosition(const Grid<T>& grid,
                                      const Vector<typename Grid<T>::SizeType>& pos) const noexcept
    {
        const Vector<float> start{-getWidth() * 0.5f, -getHeight() * 0.5f};
        const Vector<float> step{getWidth() / grid.getWidth(), getHeight() / grid.getHeight()};

        // Transform i, j coordinates to position
        // Cell center position
        const Vector<float> coord = Vector<float>(pos) + 0.5f;
        // Real position
        return start + step * coord;
    }


// Private Data Members
private:

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// Duration step
    units::Duration m_timeStep = 1.f;

    /// World size.
    Vector<units::Length> m_size{units::um(400), units::um(400)};

    /// World objects.
    ObjectContainer m_objects;

    /// World main cell radius.
    units::Length m_mainCellRadius = units::um(20);

    /// Main cell position.
    Vector<units::Length> m_mainCellPosition{units::um(0), units::um(0)};

    /// Velocity grid.
    GridVelocity m_velocityGrid;

    /// Signal grid.
    GridSignal m_signalGrid;

    /// Flow speed.
    float m_flowSpeed = 1.f;

#ifdef ENABLE_RENDER

    /// Render grid
    std::unique_ptr<render::Grid> m_renderGrid;

    /// Render grid for velocities
    std::unique_ptr<render::GridVector> m_renderGridVelocity;

    /// If velocity grid shoudl be updated.
    bool m_updateGridVelocity = false;

    /// Render grid for signal
    std::unique_ptr<render::GridValue> m_renderGridSignal;

    /// If signal grid shoudl be updated.
    bool m_updateGridSignal = false;

#endif

};

/* ************************************************************************ */

}

/* ************************************************************************ */
