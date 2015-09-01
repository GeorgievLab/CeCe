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
#include "core/Vector.hpp"
#include "render/Buffer.hpp"
#include "render/GridBase.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Rendering grid object.
 */
class Grid : public GridBase
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Grid(Context& context);


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size New grid size.
     */
    void resize(Size size) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
