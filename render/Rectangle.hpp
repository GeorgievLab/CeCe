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
#include "core/compatibility.hpp"
#include "render/Buffer.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Drawing object for rectangle.
 */
class DLL_EXPORT Rectangle
{



// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Rectangle(Context& context);


// Public Accessors
public:


    /**
     * @brief Returns if object is initialized.
     */
    bool isInitialized() const NOEXCEPT
    {
        return m_buffer.isInitialized()/* && m_texture.isInitialized() */;
    }


// Public Operators
public:


    /**
     * @brief Render at current position.
     *
     * @param context Rendering context.
     */
    void draw(Context& context) NOEXCEPT;


// Private Data Members
private:

    /// Buffer for texture rendering.
    Buffer m_buffer;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
