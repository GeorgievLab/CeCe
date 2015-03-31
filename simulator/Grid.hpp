
#pragma once

/* ************************************************************************ */

// C++
#include <vector>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Grid class.
 */
template<typename T>
class Grid
{

// Public Types
public:


    /**
     * @brief Size type.
     */
    using SizeType = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param size
     */
    explicit Grid(SizeType size)
        : Grid(size, size)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param width
     * @param height
     * @param depth
     */
    Grid(SizeType width, SizeType height)
    {
        resize(width, height);
    }


// Public Accessors
public:


    /**
     * @brief Array operator.
     *
     * @param x
     * @param y
     *
     * @return
     */
    T& operator()(SizeType x, SizeType y) noexcept
    {
        return m_data[x + y * m_width];
        //return m_data[x * m_height + y];
    }


    /**
     * @brief Array operator.
     *
     * @param x
     * @param y
     *
     * @return
     */
    const T& operator()(SizeType x, SizeType y) const noexcept
    {
        return m_data[x + y * m_width];
        //return m_data[x * m_height + y];
    }


    /**
     * @brief Returns grid width.
     *
     * @return
     */
    SizeType getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns grid height.
     *
     * @return
     */
    SizeType getHeight() const noexcept
    {
        return m_height;
    }


    /**
     * @brief Returns grid plain data.
     *
     * @return
     */
    const T* getData() const noexcept
    {
        return m_data.data();
    }


// Public Operations
public:


    /**
     * @brief Resize grid.
     *
     * @param size Grid size.
     */
    void resize(SizeType size)
    {
        resize(size, size);
    }


    /**
     * @brief Resize grid.
     *
     * @param size Grid size.
     */
    void resize(SizeType width, SizeType height)
    {
        m_width = width;
        m_height = height;
        m_data.resize(width * height);
    }


// Private Data Members
private:

    /// Grid data.
    std::vector<T> m_data;

    /// Width size (x).
    SizeType m_width;

    /// Height size (y).
    SizeType m_height;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
