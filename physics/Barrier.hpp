
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Core
#include "core/Units.hpp"
#include "core/Vect.hpp"

// Physics
#include "physics/Object.hpp"

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
    Length getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Get barrier depth.
     *
     * @return
     */
    Length getDepth() const noexcept
    {
        return m_depth;
    }


    /**
     * @brief Get barrier height.
     *
     * @return
     */
    Length getHeight() const noexcept
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
    void setWidth(Length width) noexcept
    {
        m_width = width;
    }


    /**
     * @brief Set barrier depth.
     *
     * @param depth
     */
    void setDepth(Length depth) noexcept
    {
        m_depth = depth;
    }


    /**
     * @brief Set barrier height.
     *
     * @param height
     */
    void setHeight(Length height) noexcept
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
    Length m_width;

    /// Barrier height.
    Length m_height;

    /// Barrier depth.
    Length m_depth;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
