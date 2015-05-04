
#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// Simulator
#include "core/Vector.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Rendering grid object - base class.
 */
class GridBase
{

// Public Types
public:


    /// Position type.
    using PositionType = unsigned int;


// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Vector<PositionType>& getSize() const noexcept
    {
        return m_size;
    }


// Public Operators
public:



    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void resize(Vector<PositionType> size) noexcept
    {
        m_size = std::move(size);
    }


// Private Data Members
private:


    /// Grid size.
    Vector<PositionType> m_size;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
