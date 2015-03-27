
#pragma once

/* ************************************************************************ */

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
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
    static constexpr unsigned SEGMENTS = 50;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor
     */
    Circle() noexcept;


    /**
     * @brief Render at current position.
     */
    void render() noexcept;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
