
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Drawable.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL circle object.
 */
class Circle : public Drawable
{

// Public Constants
public:

    /**
     * @brief Number of circle segments
     */
    static constexpr unsigned SEGMENTS = 100;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor
     */
    Circle() noexcept;


    /**
     * @brief Render at current position.
     *
     * @param pos    Position where to render.
     * @param radius Circle radius.
     * @param color  Circle color.
     */
    void render(const Position& pos, float radius, const Color& color) noexcept;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
