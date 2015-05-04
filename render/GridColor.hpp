
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
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
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


// Private Data Members
private:

    /// Texture.
    unsigned int m_texture;

    /// Buffer for texture rendering.
    Buffer m_buffer;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
