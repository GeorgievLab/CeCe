
#ifndef _LIBRARY_CELL_H_
#define _LIBRARY_CELL_H_

/* ************************************************************************ */

// C++
#include <cstddef>

// wxWidgets
#include <wx/dc.h>

// Box2D
#include <Box2D/Box2D.h>

// Core
#include "Units.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell
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


    /**
     * @brief Returns body for physics.
     *
     * @return
     */
    b2Body* GetPhysicsBody() const noexcept
    {
        return m_physicsBody;
    }


public:


    /**
     * @brief Spawn cell in given world.
     *
     * @param world World where to spawn.
     * @param pos   Position to spawn.
     */
    virtual void Spawn(World* world, const b2Vec2& pos) {}


    /**
     * @brief Render cell.
     *
     * @param dc Device context.
     */
    //virtual void Render(wxDC& dc) const noexcept
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

    /// Physical body.
    b2Body* m_physicsBody = nullptr;

};

/* ************************************************************************ */

#endif // _LIBRARY_CELL_H_
