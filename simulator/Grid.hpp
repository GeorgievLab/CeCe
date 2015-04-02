
#pragma once

/* ************************************************************************ */

// C++
#include <vector>

// Simulator
#include "core/Vector.hpp"

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
    explicit Grid(Vector<SizeType> size)
    {
        resize(size);
    }


    /**
     * @brief Constructor.
     *
     * @param width
     * @param height
     */
    Grid(SizeType width, SizeType height)
        : Grid(Vector<SizeType>{width, height})
    {
        // Nothing to do
    }


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


// Public Accessors
public:


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    T& operator[](const Vector<SizeType>& coord) noexcept
    {
        return m_data[coord.x + coord.y * getWidth()];
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    const T& operator[](const Vector<SizeType>& coord) const noexcept
    {
        return m_data[coord.x + coord.y * getWidth()];
    }


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
        return m_data[x + y * getWidth()];
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
        return m_data[x + y * getWidth()];
        //return m_data[x * m_height + y];
    }


    /**
     * @brief Returns grid size.
     *
     * @return
     */
    const Vector<SizeType>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Returns grid width.
     *
     * @return
     */
    SizeType getWidth() const noexcept
    {
        return m_size.width;
    }


    /**
     * @brief Returns grid height.
     *
     * @return
     */
    SizeType getHeight() const noexcept
    {
        return m_size.height;
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
     * @param width
     * @param height
     */
    void resize(SizeType width, SizeType height)
    {
        resize({width, height});
    }


    /**
     * @brief Resize grid.
     *
     * @param size Grid size.
     */
    void resize(Vector<SizeType> size)
    {
        m_size = std::move(size);
        m_data.resize(m_size.width * m_size.height);
    }


    /**
     * @brief Check if coordinates are in range.
     *
     * @param coord
     *
     * @return
     */
    bool inRange(Vector<SizeType> coord) const noexcept
    {
        return
            coord.x >= 0 &&
            coord.y >= 0 &&
            coord.x < getWidth() &&
            coord.y < getHeight()
        ;
    }


    /**
     * @brief Check if coordinates are in range.
     *
     * @param x
     * @param y
     *
     * @return
     */
    bool inRange(SizeType x, SizeType y) const noexcept
    {
        return inRange({x, y});
    }


// Private Data Members
private:

    /// Grid data.
    std::vector<T> m_data;

    /// Grid size
    Vector<SizeType> m_size;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
