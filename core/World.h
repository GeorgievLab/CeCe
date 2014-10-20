
#ifndef _LIBRARY_WORLD_H_
#define _LIBRARY_WORLD_H_

/* ************************************************************************ */

// C++
#include <vector>
#include <memory>
#include <cassert>

// Box2D
#include <Box2D/Box2D.h>

// Library
#include "Cell.h"
#include "Barrier.h"

/* ************************************************************************ */

/**
 * @brief World for cells.
 */
class World
{
public:


    /// Cell container type.
    using CellContainer = std::vector<std::unique_ptr<Cell>>;

    /// Barrier container type.
    using BarrierContainer = std::vector<Barrier>;


public:


    /**
     * @brief Constructor.
     */
    World() noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~World();


public:


    /**
     * @brief Return physical world.
     *
     * @return
     */
    b2World& GetPhysicsWorld() noexcept
    {
        return m_physicsWorld;
    }


    /**
     * @brief Return physical world.
     *
     * @return
     */
    const b2World& GetPhysicsWorld() const noexcept
    {
        return m_physicsWorld;
    }


    /**
     * @brief Return cell container.
     */
    const CellContainer& GetCells() const noexcept
    {
        return m_cells;
    }


    /**
     * @brief Find cell by ID.
     *
     * @param cellId
     *
     * @return
     */
    Cell* FindCell(Cell::Id cellId) noexcept;


    /**
     * @brief Find cell by ID.
     *
     * @param cellId
     *
     * @return
     */
    const Cell* FindCell(Cell::Id cellId) const noexcept;


public:


    /**
     * @brief Add a new cell to population.
     *
     * @param cell
     */
    void AddCell(std::unique_ptr<Cell> cell)
    {
        assert(cell);
        // Sort only when last element is not less than the new element
        m_sorted = m_cells.empty() || m_cells.back()->GetId() < cell->GetId();
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
        m_barriers.emplace_back(this, std::forward<Args>(args)...);
    }


    /**
     * @brief Remove cell.
     *
     * @param cell
     */
    void RemoveCell(Cell::Id cellId)
    {
        m_cells.erase(FindCellPosition(cellId));
        // Remove element from sorted list gives still a sorted list
    }


    /**
     * @brief Remove cell.
     *
     * @param cell
     */
    void RemoveCell(Cell* cell)
    {
        assert(cell);
        RemoveCell(cell->GetId());
    }


    /**
     * @brief Find cell by ID.
     *
     * @param cellId
     *
     * @return
     */
    CellContainer::iterator FindCellPosition(Cell::Id cellId) noexcept;


    /**
     * @brief Find cell by ID.
     *
     * @param cellId
     *
     * @return
     */
    CellContainer::const_iterator FindCellPosition(Cell::Id cellId) const noexcept;


public:


    /**
     * @brief Sort list of cells by IDs.
     */
    void Sort() noexcept;


    /**
     * @brief Update world.
     */
    void Update() noexcept;


    /**
     * @brief Render world.
     *
     * @param dc Device context.
     */
    //void Render(wxDC& dc) const noexcept;
    void Render() const noexcept;


private:

    /// Sorted flag.
    bool m_sorted = true;

    /// Cell population
    CellContainer m_cells;

    /// Barriers
    BarrierContainer m_barriers;

    /// Physical world.
    b2World m_physicsWorld;

};

/* ************************************************************************ */

#endif // _LIBRARY_WORLD_H_
