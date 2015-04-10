
#pragma once

/* ************************************************************************ */

// Simulator
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Buffer.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL circle object.
 */
class Circle
{

// Public Constants
public:

    /**
     * @brief Number of circle segments
     */
    static constexpr unsigned SEGMENTS = 100;


// Public Operations
public:


    /**
     * @brief Initialize.
     */
    void init();


    /**
     * @brief Render at current position.
     *
     * @param pos    Position where to render.
     * @param radius Circle radius.
     * @param color  Circle color.
     */
    void draw(const Position& pos, float radius, const Color& color) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
