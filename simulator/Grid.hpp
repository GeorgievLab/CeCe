
#pragma once

/* ************************************************************************ */

// C++
#include <vector>

// Simulator
#include "core/Vector.hpp"
#include "core/AlignedAllocator.hpp"

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


    /**
     * @brief Container type.
     */
    using ContainerType = std::vector<T, AlignedAllocator<T, Alignment::AVX>>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param size
     */
    explicit Grid(Vector<SizeType> size)
    {
        resize(std::move(size));
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


// Public Operators
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
        return m_data[calcOffset(coord)];
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
        return m_data[calcOffset(coord)];
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
        return m_data[calcOffset(x, y)];
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
        return m_data[calcOffset(x, y)];
    }


// Public Accessors
public:


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
        return m_size.getWidth();
    }


    /**
     * @brief Returns grid height.
     *
     * @return
     */
    SizeType getHeight() const noexcept
    {
        return m_size.getHeight();
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


    /**
     * @brief Returns grid inner container.
     *
     * @return
     */
    const ContainerType& getContainer() const noexcept
    {
        return m_data;
    }


    /**
     * @brief Returns grid inner container.
     *
     * @return
     */
    ContainerType& getContainer() noexcept
    {
        return m_data;
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename ContainerType::iterator begin() noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename ContainerType::iterator end() noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end constant iterator.
     *
     * @return
     */
    typename ContainerType::const_iterator cend() const noexcept
    {
        return m_data.cend();
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
        m_data.resize(m_size.getWidth() * m_size.getHeight());
    }


    /**
     * @brief Check if coordinates are in range.
     *
     * @param coord
     *
     * @return
     */
    bool inRange(const Vector<SizeType>& coord) const noexcept
    {
        return
            coord.getX() >= 0 &&
            coord.getY() >= 0 &&
            coord.getX() < getWidth() &&
            coord.getY() < getHeight()
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


    /**
     * @brief Calculate array offset.
     *
     * @param coord
     *
     * @return
     */
    SizeType calcOffset(const Vector<SizeType>& coord) const noexcept
    {
        return coord.getX() + coord.getY() * getWidth();
    }

    /**
     * @brief Calculate array offset.
     *
     * @param x
     * @param y
     *
     * @return
     */
    SizeType calcOffset(SizeType x, SizeType y) const noexcept
    {
        return calcOffset({x, y});
    }


// Private Data Members
private:

    /// Grid data.
    ContainerType m_data;

    /// Grid size
    Vector<SizeType> m_size;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
