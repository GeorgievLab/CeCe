
#ifndef _SIMULATOR_CELL_H_
#define _SIMULATOR_CELL_H_

/* ************************************************************************ */

// C++
#include <cstddef>
#include <memory>

// Core
#include "core/Units.h"

// Physics
#include "physics/Cell.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell : public physics::Cell
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
    Id getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Returns world.
     */
    World* getWorld() const noexcept
    {
        return m_world;
    }


// Private Data Members
private:

    /// Unique cell ID.
    static Id s_id;

    /// Cell ID.
    Id m_id;

    /// Cell world.
    World* m_world;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_CELL_H_
