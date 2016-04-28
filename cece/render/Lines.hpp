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
#include "cece/core/Vector.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/render/Buffer.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;
class Color;

/* ************************************************************************ */

/**
 * @brief Render object for drawing lines.
 */
class CECE_EXPORT Lines
{


// Public Types
public:


    /// Point type
    using PointType = Vector<float>;

    /// Line type.
    using LineType = Pair<PointType, PointType>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     */
    explicit Lines(Context& context)
        : m_buffer(context)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param lines
     */
    Lines(Context& context, const DynamicArray<Pair<PointType, PointType>>& lines)
        : m_buffer(context)
    {
        add(lines);
    }


// Public Mutators
public:


    /**
     * @brief Add new lines.
     *
     * @param lines
     */
    void add(const DynamicArray<Pair<PointType, PointType>>& lines);


    /**
     * @brief Remove all lines.
     */
    void clear()
    {
        m_count = 0;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     * @param color
     */
    void draw(Context& context, const Color& color) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Number of lines.
    std::size_t m_count = 0;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
