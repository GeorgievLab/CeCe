
#ifndef _LIBRARY_CELL_H_
#define _LIBRARY_CELL_H_

/* ************************************************************************ */

// C++
#include <cstddef>

// Core
#include "Units.h"
#include "CellPhysics.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell : public CellPhysics
{
public:


    /// Cell ID type.
    using Id = std::size_t;


public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     */
    Cell(World* world, MicroMeters x, MicroMeters y);


    /**
     * @brief Destructor.
     */
    virtual ~Cell();


public:


    /**
     * @brief Operator for sorting.
     *
     * @param cell
     */
    bool operator<(const Cell& cell) const noexcept
    {
        return m_id < cell.m_id;
    }


public:


    /**
     * @brief Returns ID.
     *
     * @return
     */
    Id GetId() const noexcept
    {
        return m_id;
    }


public:


    /**
     * @brief Render cell.
     */
    virtual void Render() const noexcept
    {
        // Nothing to do
    }


private:


    /// Unique cell ID.
    static Id s_id;

    /// Cell ID.
    Id m_id;

    /// Cell world.
    World* m_world;

};

/* ************************************************************************ */

#endif // _LIBRARY_CELL_H_
