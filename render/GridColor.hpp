
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>

// Simulator
#include "core/compatibility.hpp"
#include "core/Vector.hpp"
#include "core/Grid.hpp"
#include "render/Buffer.hpp"
#include "render/Color.hpp"
#include "render/Texture.hpp"
#include "render/GridBase.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture-like grid.
 */
class DLL_EXPORT GridColor : public GridBase
{



// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit GridColor(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size
     */
    GridColor(Context& context, core::Vector<PositionType> size);


// Public Operators
public:


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& operator[](const core::Vector<PositionType>& coord) NOEXCEPT
    {
        return get(coord);
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& operator[](const core::Vector<PositionType>& coord) const NOEXCEPT
    {
        return get(coord);
    }


// Public Accessors
public:


    /**
     * @brief Returns if object is initialized.
     */
    bool isInitialized() const NOEXCEPT
    {
        return m_buffer.isInitialized()/* && m_texture.isInitialized() */;
    }


    /**
     * @brief Get mutable pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    Color& get(const core::Vector<PositionType>& coord) NOEXCEPT
    {
        return m_colors[coord];
    }


    /**
     * @brief Get pixel color.
     *
     * @param coord Pixel coordinates.
     * @param color Pixel color.
     */
    const Color& get(const core::Vector<PositionType>& coord) const NOEXCEPT
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
    void set(const core::Vector<PositionType>& coord, const Color& color) NOEXCEPT
    {
        // Set color
        m_colors[coord] = color;
        m_colorsUpdated = true;
    }


    /**
     * @brief Set flag that indicates the texture on GPU should be updated.
     */
    void colorsUpdated() NOEXCEPT
    {
        m_colorsUpdated = true;
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) NOEXCEPT;


    /**
     * @brief Resize grid.
     *
     * @param size  New grid size.
     * @param color Background color.
     */
    void resize(core::Vector<PositionType> size, const Color& color = colors::BLACK);


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


// Private Data Members
private:

    /// Texture.
    Texture m_texture;

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
