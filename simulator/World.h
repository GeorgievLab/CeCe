
#ifndef _SIMULATOR_WORLD_H_
#define _SIMULATOR_WORLD_H_

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>

// Physics
#include "physics/World.h"

// Simulator
#include "simulator/Cell.h"

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
    using CellContainer = std::vector<std::unique_ptr<Cell>>;

    /// Barrier container type.
    using BarrierContainer = std::vector<std::unique_ptr<Barrier>>;


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
     * @brief Return cell container.
     *
     * @return
     */
    const CellContainer& getCells() const noexcept
    {
        return m_cells;
    }


    /**
     * @brief Returns world width.
     *
     * @return
     */
    MicroMeters getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns world height.
     *
     * @return
     */
    MicroMeters getHeight() const noexcept
    {
        return m_height;
    }


// Public Mutators
public:


    /**
     * @brief Resize world.
     *
     * @param width
     * @param height
     */
    void resize(MicroMeters width, MicroMeters height) noexcept
    {
        m_width = width;
        m_height = height;
    }


    /**
     * @brief Add a new cell to population.
     *
     * @param cell
     */
    Cell* addCell(std::unique_ptr<Cell> cell)
    {
        assert(cell);
        m_cells.push_back(std::move(cell));
        return m_cells.back().get();
    }


    /**
     * @brief Create a new cell to population.
     *
     * @param cell
     *
     * @return
     */
    template<typename T, typename... Args>
    Cell* newCell(Args&&... args)
    {
        return addCell(std::unique_ptr<T>(new T(this, std::forward<Args>(args)...)));
    }


    /**
     * @brief Create a new barrier.
     *
     * @param args...
     */
    template<typename... Args>
    void newBarrier(Args&&... args)
    {
        //m_barriers.emplace_back(new Barrier(this, std::forward<Args>(args)...));
    }


// Public Operations
public:


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
    void update(float step) noexcept;


// Private Data Members
private:

    /// World width.
    MicroMeters m_width{400};

    /// World height.
    MicroMeters m_height{400};

    /// Cell population
    CellContainer m_cells;

    /// Barriers
    //BarrierContainer m_barriers;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_H_
