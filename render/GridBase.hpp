
#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// Simulator
#include "core/compatibility.hpp"
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
    const core::Vector<PositionType>& getSize() const NOEXCEPT
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
    void resize(core::Vector<PositionType> size) NOEXCEPT
    {
        m_size = std::move(size);
    }


// Private Data Members
private:


    /// Grid size.
    core::Vector<PositionType> m_size;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
