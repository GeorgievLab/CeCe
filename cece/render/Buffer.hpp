/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief GPU buffer.
 */
class CECE_EXPORT Buffer
{

// Public Types
public:


    /**
     * @brief Buffer ID type.
     */
    using Id = unsigned int;


    /**
     * @brief Buffer size type.
     */
    using SizeType = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Buffer(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size    Buffer size.
     * @param data    Optional buffer data.
     */
    Buffer(Context& context, SizeType size, const void* data = nullptr);


    /**
     * @brief Destructor.
     */
    ~Buffer();


// Public Accessors
public:


    /**
     * @brief Return buffer ID.
     *
     * @return
     */
    Id getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Return if buffer is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept
    {
        return m_id != 0;
    }


// Public Operators
public:


    /**
     * @brief Resize buffer object.
     *
     * @param context Rendering context.
     * @param size    Buffer size.
     * @param data    Optional buffer data.
     */
    void resize(SizeType size, const void* data = nullptr);


// Private Data Members
private:

    /// Buffer identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
