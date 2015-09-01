/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/DynamicArray.hpp"
#include "render/VertexElement.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Description of vertices stored in vertex buffer.
 */
class VertexFormat
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

/* ************************************************************************ */
