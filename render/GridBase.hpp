/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

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

// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Size& getSize() const noexcept
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
    void resize(Size size) noexcept
    {
        m_size = std::move(size);
    }


// Private Data Members
private:


    /// Grid size.
    Size m_size;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
