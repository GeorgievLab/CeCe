
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>

// Simulator
#include "simulator/Velocity.hpp"
#include "simulator/Object.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
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

// Public Structures
public:


    /**
     * @brief Grid cell structure.
     */
    struct GridCell
    {
        Velocity velocity;
    };


// Public Types
public:


    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;

    /// Log callback function.
    using LogFunction = std::function<void(const std::string&)>;


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
     * @brief Returns world width.
     *
     * @return
     */
    units::Length getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    units::Length getHeight() const noexcept
    {
        return m_height;
    }


    /**
     * @brief Returns log callback function.
     *
     * @return
     */
    const LogFunction& getLogFunction() const noexcept
    {
        return m_logFunction;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    Grid<GridCell>& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const Grid<GridCell>& getGrid() const noexcept
    {
        return m_grid;
    }


// Public Mutators
public:


    /**
     * @brief Set log callback function.
     *
     * @param fn
     */
    void setLogFunction(LogFunction fn) noexcept
    {
        m_logFunction = std::move(fn);
    }


    /**
     * @brief Resize world.
     *
     * @param width
     * @param height
     */
    void resize(units::Length width, units::Length height) noexcept
    {
        m_width = width;
        m_height = height;
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


// Public Operations
public:


    /**
     * @brief Clean world data.
     */
    virtual void clean();


    /**
     * @brief Reset world.
     */
    virtual void reset();


    /**
     * @brief Update world.
     *
     * @param step
     */
    virtual void update(units::Duration step) noexcept;

#ifdef ENABLE_RENDER

    /**
     * @brief Render world.
     *
     * @param context
     */
    virtual void render(render::Context& context);

#endif

// Private Data Members
private:

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// World width.
    units::Length m_width = units::um(400);

    /// World height.
    units::Length m_height = units::um(400);

    /// World objects.
    ObjectContainer m_objects;

    /// Log function.
    LogFunction m_logFunction;

    /// Signal grid.
    Grid<GridCell> m_grid;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
