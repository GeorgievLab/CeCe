
#pragma once

/* ************************************************************************ */

// Simulator
#include "simulator/Object.hpp"
#include "simulator/Position.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Static simulation object with known static position.
 *
 * Static object doesn't need update, because is static in time.
 */
class StaticObject : public Object
{

// Public Accessors
public:


    /**
     * @brief Returns current position.
     *
     * @return
     */
    const Position& getPosition() const noexcept
    {
        return m_position;
    }


// Public Mutators
public:


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(Position pos) noexcept
    {
        m_position = std::move(pos);
    }


// Private Data Members
private:

    /// Object position
    Position m_position;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
