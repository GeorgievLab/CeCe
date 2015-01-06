
#ifndef _SIMULATOR_CELL_H_
#define _SIMULATOR_CELL_H_

/* ************************************************************************ */

// C++
#include <cstddef>
#include <memory>
#include <functional>

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

    /// Behaviour function type.
    using BehaviourFn = std::function<void(Cell&)>;

public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     * @param fn
     */
    Cell(World* world, MicroMeters x, MicroMeters y, BehaviourFn fn);


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
     *
     * @return
     */
    World* getWorld() const noexcept
    {
        return m_world;
    }


    /**
     * @brief Returns cell behaviour function.
     *
     * @return
     */
    const BehaviourFn& getBehaviour() const noexcept
    {
        return m_behaviour;
    }


    unsigned int getGfp() const noexcept
    {
        return m_gfp;
    }


    unsigned int getRfp() const noexcept
    {
        return m_rfp;
    }


    unsigned int getYfp() const noexcept
    {
        return m_yfp;
    }


public:


    /**
     * @brief Set cell behaviour function.
     *
     * @param fn Function.
     */
    void setBehaviour(BehaviourFn fn) noexcept
    {
        m_behaviour = std::move(fn);
    }


    void setGfp(unsigned int gfp) noexcept
    {
        m_gfp = gfp;
    }


    void setRfp(unsigned int rfp) noexcept
    {
        m_rfp = rfp;
    }


    void setYfp(unsigned int yfp) noexcept
    {
        m_yfp = yfp;
    }


// Private Data Members
private:

    /// Unique cell ID.
    static Id s_id;

    /// Cell ID.
    Id m_id;

    /// Cell world.
    World* m_world;

    /// Behaviour function.
    BehaviourFn m_behaviour;

    /// Number of GFP proteins.
    unsigned int m_gfp = 0;

    /// Number of RFP proteins.
    unsigned int m_rfp = 0;

    /// Number of YFP proteins.
    unsigned int m_yfp = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_CELL_H_
