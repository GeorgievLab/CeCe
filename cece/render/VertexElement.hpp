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
#include "cece/render/datatype.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Types used for vertex format definition elements.
 */
enum class VertexElementType
{
    /// Position.
    Position,
    /// Color.
    Color,
    /// Normal.
    Normal,
    /// Texture coordinates.
    TexCoord
};

/* ************************************************************************ */

/**
 * @brief Vertex format element.
 */
class CECE_EXPORT VertexElement
{

// Public Ctors
public:


    /**
     * @brief Constructor
     *
     * @param type     Element type.
     * @param dataType Data type.
     * @param count    Number of values.
     */
    constexpr VertexElement(VertexElementType type, DataType dataType, unsigned int count)
        : m_type(type)
        , m_dataType(dataType)
        , m_count(count)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns element type.
     *
     * @return
     */
    constexpr VertexElementType getType() const noexcept
    {
        return m_type;
    }


    /**
     * @brief Returns value data type.
     *
     * @return
     */
    constexpr DataType getDataType() const noexcept
    {
        return m_dataType;
    }


    /**
     * @brief Returns number of values.
     *
     * @return
     */
    constexpr unsigned int getCount() const noexcept
    {
        return m_count;
    }


// Public Operations
public:


    /**
     * @brief Calculate element size.
     *
     * @return
     */
    constexpr size_t getSize() const noexcept
    {
        return getCount() * data_type_size(getDataType());
    }


// Private Data Members
private:


    /// Type of element.
    VertexElementType m_type;

    /// Component data type.
    DataType m_dataType;

    /// A number of components.
    unsigned int m_count;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
