/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

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
class DLL_EXPORT GridBase
{

// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Size& getSize() const NOEXCEPT
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
    void resize(Size size) NOEXCEPT
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
