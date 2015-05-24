
#pragma once

/* ************************************************************************ */

// C++
#include <vector>

// Simulator
#include "core/compatibility.hpp"
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
#if _MSC_VER
		: m_elements(elements.begin(), elements.end())
#else
        : m_elements{elements}
#endif
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
    const VertexElement& operator[](unsigned int pos) const NOEXCEPT
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
	size_t getCount() const NOEXCEPT
    {
        return m_elements.size();
    }


    /**
     * @brief Returns if format is empty.
     *
     * @return
     */
    bool isEmpty() const NOEXCEPT
    {
        return m_elements.empty();
    }


    /**
     * @brief Calculates format size in bytes.
     *
     * @return
     */
	size_t getSize() const NOEXCEPT
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
	 const std::vector<VertexElement>& getElements() const NOEXCEPT
    {
        return m_elements;
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
	 std::vector<VertexElement>::const_iterator begin() const NOEXCEPT
    {
        return m_elements.begin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
	 std::vector<VertexElement>::const_iterator end() const NOEXCEPT
    {
        return m_elements.end();
    }


// Private Data Members
private:

    /// Stored elements.
    std::vector<VertexElement> m_elements;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
