
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>

// Simulator
#include "core/Vector.hpp"
#include "core/Grid.hpp"
#include "render/Buffer.hpp"
#include "render/Color.hpp"
#include "render/GridBase.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture-like grid.
 */
class GridColor : public GridBase
{


// Public Operators
public:


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& operator[](const Vector<PositionType>& coord) noexcept
    {
        return get(coord);
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& operator[](const Vector<PositionType>& coord) const noexcept
    {
        return get(coord);
    }


// Public Accessors
public:


    /**
     * @brief Returns if object is initialized.
     */
    bool isInitialized() const noexcept
    {
        return m_buffer.isInitialized()/* && m_texture.isInitialized() */;
    }


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& get(const Vector<PositionType>& coord) noexcept
    {
        return m_colors[coord];
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& get(const Vector<PositionType>& coord) const noexcept
    {
        return m_colors[coord];
    }


// Public Mutators
public:


    /**
     * @brief Set pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    void set(const Vector<PositionType>& coord, const Color& color) noexcept
    {
        // Set color
        m_colors[coord] = color;
        m_colorsUpdated = true;
    }


    /**
     * @brief Set flag that indicates the texture on GPU should be updated.
     */
    void colorsUpdated() noexcept
    {
        m_colorsUpdated = true;
    }


// Public Operators
public:


    /**
     * @brief Initialize.
     *
     * @param context Rendering context.
     */
    void init(Context& context);


    /**
     * @brief Initialize.
     *
     * @param context Rendering context.
     * @param size
     */
    void init(Context& context, Vector<PositionType> size);


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void resize(Vector<PositionType> size);


    /**
     * @brief Clear grid to default color.
     *
     * @param color Clear color.
     */
    void clear(const Color& color);


    /**
     * @brief Synchronize local buffer with GPU texture.
     */
    void sync();


    /**
     * @brief Release.
     */
    void release()
    {
        //m_texture.release();
        m_buffer.release();
    }


// Private Data Members
private:

    /// Texture.
    unsigned int m_texture;

    /// Buffer for texture rendering.
    Buffer m_buffer;

    /// Buffer for storing texture data.
    core::Grid<Color> m_colors;

    /// If colors were updated.
    bool m_colorsUpdated = false;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
