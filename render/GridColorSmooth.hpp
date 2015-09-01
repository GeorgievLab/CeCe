/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "render/GridColor.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture-like grid with color interpolation.
 */
class GridColorSmooth : public GridColor
{



// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit GridColorSmooth(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size
     */
    GridColorSmooth(Context& context, Size size)
        : GridColorSmooth(context)
    {
        resize(std::move(size));
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


// Private Data Members
private:


    /// Vertex shader.
    Shader m_vertexShader;

    /// Fragment shader.
    Shader m_fragmentShader;

    /// Shader program.
    Program m_program;

    /// Size variable pointer.
    Program::UniformId m_sizePtr;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
