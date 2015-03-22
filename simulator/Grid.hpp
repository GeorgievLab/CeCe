
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

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param size
     */
    explicit Grid(unsigned int size)
        : Grid(size, size, size)
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
    Grid(unsigned int width, unsigned int height, unsigned int depth)
        : m_width(width)
        , m_height(height)
        , m_depth(depth)
    {
        m_data.resize(width * height * depth);
    }


// Public Accessors
public:


    /**
     * @brief Array operator.
     *
     * @param x
     * @param y
     * @param z
     *
     * @return
     */
    T& operator()(unsigned int x, unsigned int y, unsigned int z) noexcept
    {
        return m_data[x + y * m_width + z * m_width * m_height];
    }


    /**
     * @brief Array operator.
     *
     * @param x
     * @param y
     * @param z
     *
     * @return
     */
    const T& operator()(unsigned int x, unsigned int y, unsigned int z) const noexcept
    {
        return m_data[x + y * m_width + z * m_width * m_height];
    }


    /**
     * @brief Returns grid width.
     *
     * @return
     */
    unsigned int getWidth() const noexcept
    {
        return m_width;
    }


    /**
     * @brief Returns grid height.
     *
     * @return
     */
    unsigned int getHeight() const noexcept
    {
        return m_height;
    }


    /**
     * @brief Returns grid depth.
     *
     * @return
     */
    unsigned int getDepth() const noexcept
    {
        return m_depth;
    }


// Private Data Members
private:

    /// Grid data.
    std::vector<T> m_data;

    /// Width size (x).
    unsigned int m_width;

    /// Height size (y).
    unsigned int m_height;

    /// Depth size (z).
    unsigned int m_depth;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
