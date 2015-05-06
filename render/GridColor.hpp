
#pragma once

/* ************************************************************************ */

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


// Public Mutators
public:


    /**
     * @brief Set cell color.
     *
     * @param coord Coordinates.
     * @param color Cell color.
     */
    void set(const Vector<PositionType>& coord, const Color& color) noexcept;


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
