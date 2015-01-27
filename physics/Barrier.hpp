
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Core
#include "core/Units.h"
#include "core/Vect.h"

// Physics
#include "physics/Object.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Barrier physics.
 */
class Barrier : public Object
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world Physical world.
     */
    explicit Barrier(World* world);


// Public Accessors
public:


    /**
     * @brief Get barrier width.
     *
     * @return
     */
    MicroMeters getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Get barrier depth.
     *
     * @return
     */
    MicroMeters getDepth() const noexcept
    {
        return m_depth;
    }


    /**
     * @brief Get barrier height.
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
     * @brief Set barrier width.
     *
     * @param width
     */
    void setWidth(MicroMeters width) noexcept
    {
        m_width = width;
    }


    /**
     * @brief Set barrier depth.
     *
     * @param depth
     */
    void setDepth(MicroMeters depth) noexcept
    {
        m_depth = depth;
    }


    /**
     * @brief Set barrier height.
     *
     * @param height
     */
    void setHeight(MicroMeters height) noexcept
    {
        m_height = height;
    }


// Public Operations
public:


    /**
     * @brief Update barrier body.
     */
    void updateBody() noexcept;


// Private Data Members
private:


    /// Barrier width.
    MicroMeters m_width;

    /// Barrier height.
    MicroMeters m_height;

    /// Barrier depth.
    MicroMeters m_depth;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
