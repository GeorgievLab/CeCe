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
#include "cece/core/DynamicArray.hpp"
#include "cece/render/VertexElement.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Description of vertices stored in vertex buffer.
 */
class CECE_EXPORT VertexFormat
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    VertexFormat() = default;


    /**
     * @brief Constructor.
     *
     * @param elements A list of source elements.
     */
    VertexFormat(std::initializer_list<VertexElement> elements)
        : m_elements{elements}
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Returns a reference to vertex element.
     *
     * @param pos Required position.
     *
     * @return A reference to vertex element.
     */
    const VertexElement& operator[](unsigned int pos) const noexcept
    {
        return m_elements[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns a reference to vertex element.
     *
     * @param pos Required position.
     *
     * @return
     */
    const VertexElement& at(unsigned int pos) const
    {
        return m_elements.at(pos);
    }


    /**
     * @brief Returns number of elements.
     *
     * @return
     */
    size_t getCount() const noexcept
    {
        return m_elements.size();
    }


    /**
     * @brief Returns if format is empty.
     *
     * @return
     */
    bool isEmpty() const noexcept
    {
        return m_elements.empty();
    }


    /**
     * @brief Calculates format size in bytes.
     *
     * @return
     */
    size_t getSize() const noexcept
    {
        size_t sum = 0;

        for (auto& element : m_elements)
            sum += element.getSize();

        return sum;
    }


    /**
     * @brief Returns mutable reference to elements.
     *
     * @return
     */
    const DynamicArray<VertexElement>& getElements() const noexcept
    {
        return m_elements;
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<VertexElement>::const_iterator begin() const noexcept
    {
        return m_elements.begin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<VertexElement>::const_iterator end() const noexcept
    {
        return m_elements.end();
    }


// Private Data Members
private:

    /// Stored elements.
    DynamicArray<VertexElement> m_elements;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
