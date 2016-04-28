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
#include "cece/core/Real.hpp"
#include "cece/core/Vector.hpp"
#include "cece/render/Buffer.hpp"
#include "cece/render/GridBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief OpenGL grid for vectors object.
 */
class CECE_EXPORT GridVector : public GridBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     * @param max     Maximum value.
     */
    GridVector(Context& context, Size size, const Vector<RealType>* data, RealType max = 1);


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     * @param max     Maximum value.
     */
    template<typename T>
    GridVector(Context& context, Size size, const Vector<T>* data, RealType max = 1)
        : GridVector(context, std::move(size), reinterpret_cast<const Vector<RealType>*>(data), max)
    {
        static_assert(sizeof(T) == sizeof(RealType), "T must have same size as RealType");
    }


// Public Mutators
public:


    /**
     * @brief Set maximum value.
     *
     * @param max
     */
    void setMax(RealType max) noexcept
    {
        m_max = max;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     */
    void draw(Context& context) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(Size size, const Vector<RealType>* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const Vector<RealType>* data) noexcept;


    /**
     * @brief Update data.
     *
     * @tparam T Vector type.
     *
     * @param data
     */
    template<typename T>
    void update(const Vector<T>* data) noexcept
    {
        static_assert(sizeof(T) == sizeof(RealType), "T must have same size as RealType");
        update(reinterpret_cast<const Vector<RealType>*>(data));
    }


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Maximum value.
    RealType m_max = 1;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
