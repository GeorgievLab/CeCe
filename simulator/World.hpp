
#ifndef _SIMULATOR_WORLD_H_
#define _SIMULATOR_WORLD_H_

/* ************************************************************************ */

// C++
#include <vector>
#include <list>
#include <memory>
#include <cassert>

// Physics
#include "physics/World.hpp"

// Simulator
#include "simulator/Cell.hpp"
#include "simulator/Barrier.hpp"
#include "simulator/Grid.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Barrier;

/* ************************************************************************ */

/**
 * @brief World for cells.
 */
class World : public physics::World
{
public:


    /// Cell container type.
    using CellContainer = std::list<std::unique_ptr<Cell>>;

    /// Barrier container type.
    using BarrierContainer = std::vector<std::unique_ptr<Barrier>>;

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
    unsigned long long getStepNumber() const noexcept
    {
        return m_stepNumber;
    }


    /**
     * @brief Return cell container.
     *
     * @return
     */
    const CellContainer& getCells() const noexcept
    {
        return m_cells;
    }


    /**
     * @brief Return barriers container.
     *
     * @return
     */
    const BarrierContainer& getBarriers() const noexcept
    {
        return m_barriers;
    }


    /**
     * @brief Returns world width.
     *
     * @return
     */
    Length getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    Length getHeight() const noexcept
    {
        return m_height;
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    Length getDepth() const noexcept
    {
        return m_depth;
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
    void resize(Length width, Length height) noexcept
    {
        m_width = width;
        m_height = height;
    }


    /**
     * @brief Add a new cell to population.
     *
     * @param cell
     */
    template<typename T>
    T* addCell(std::unique_ptr<T> cell)
    {
        assert(cell);
        m_cells.push_back(std::move(cell));
        return static_cast<T*>(m_cells.back().get());
    }


    /**
     * @brief Create a new cell to population.
     *
     * @param cell
     *
     * @return
     */
    template<typename T, typename... Args>
    T* newCell(Args&&... args)
    {
        return addCell(std::unique_ptr<T>(new T(this, std::forward<Args>(args)...)));
    }


    /**
     * @brief Add a new barrier.
     *
     * @param barrier
     */
    Barrier* addBarrier(std::unique_ptr<Barrier> barrier)
    {
        assert(barrier);
        m_barriers.push_back(std::move(barrier));
        return m_barriers.back().get();
    }


    /**
     * @brief Create a new barrier.
     *
     * @param args...
     */
    template<typename T, typename... Args>
    T* newBarrier(Args&&... args)
    {
        return addBarrier(std::unique_ptr<T>(new T(this, std::forward<Args>(args)...)));
    }


#ifdef ENABLE_RENDER

    /**
     * @brief Set render context.
     *
     * @param context
     */
    virtual void setContext(render::Context& context)
    {
        // Nothing to do
    }

#endif

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
     * @brief Load world code.
     *
     * @param source
     */
    virtual void load(std::string source) = 0;


    /**
     * @brief Update world.
     *
     * @param step
     */
    virtual void update(float step) noexcept;

#ifdef ENABLE_RENDER

    /**
     * @brief Render world.
     *
     * @param context
     */
    virtual void draw(render::Context& context);

#endif

// Private Data Members
private:

    /// Number of simulation steps.
    unsigned long long m_stepNumber = 0;

    /// World width.
    Length m_width{400_um};

    /// World height.
    Length m_height{400_um};

    /// World depth.
    Length m_depth{400_um};

    /// Cell population
    CellContainer m_cells;

    /// Barriers
    BarrierContainer m_barriers;

    /// Log function.
    LogFunction m_logFunction;

    /// Signal grid.
    Grid<int> m_signalGrid;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_H_
