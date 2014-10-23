
#ifndef _SIMULATOR_WORLD_H_
#define _SIMULATOR_WORLD_H_

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>
#include <mutex>

// Physics
#include "physics/World.h"

// Simulator
#include "simulator/Cell.h"

/* ************************************************************************ */

namespace render { class Context; }

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Barrier;

/* ************************************************************************ */

/**
 * @brief World implementation data.
 */
struct WorldImplData
{
    virtual ~WorldImplData() {}
};

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


public:


    /**
     * @brief Constructor.
     */
    World(std::unique_ptr<WorldImplData> data = nullptr) noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~World();


public:


    /**
     * @brief Return cell container.
     */
    const CellContainer& GetCells() const noexcept
    {
        return m_cells;
    }

public:


    /**
     * @brief Add a new cell to population.
     *
     * @param cell
     */
    void AddCell(std::unique_ptr<Cell> cell)
    {
        assert(cell);
        m_cells.push_back(std::move(cell));
    }


    /**
     * @brief Create a new cell to population.
     *
     * @param cell
     */
    template<typename T, typename... Args>
    void NewCell(Args&&... args)
    {
        AddCell(std::unique_ptr<T>(new T(this, std::forward<Args>(args)...)));
    }


    /**
     * @brief Create a new barrier.
     *
     * @param args...
     */
    template<typename... Args>
    void NewBarrier(Args&&... args)
    {
        //m_barriers.emplace_back(new Barrier(this, std::forward<Args>(args)...));
    }


// Public Operations
public:


    /**
     * @brief Update world.
     */
    void update() noexcept;


    /**
     * @brief Render world.
     *
     * @param context
     */
    void draw(render::Context& context) const noexcept;


// Private Data Members
private:

    /// Mutex for shared access.
    mutable std::mutex m_accessMutex;

    /// World implementation data.
    std::unique_ptr<WorldImplData> m_implData;

    /// Cell population
    CellContainer m_cells;

    /// Barriers
    //BarrierContainer m_barriers;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_WORLD_H_
