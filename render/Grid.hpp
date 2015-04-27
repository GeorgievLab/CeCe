
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
#include "render/Buffer.hpp"
#include "render/Color.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief OpenGL grid object.
 */
class Grid
{

// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const Vector<unsigned int>& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Returns grid width.
     *
     * @return
     */
    unsigned int getWidth() const noexcept
    {
        return m_size.getWidth();
    }


    /**
     * @brief Returns grid height.
     *
     * @return
     */
    unsigned int getHeight() const noexcept
    {
        return m_size.getHeight();
    }


    /**
     * @brief If grid rendering is enabled.
     *
     * @return
     */
    bool isRenderGrid() const noexcept
    {
        return m_renderGrid;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable grid rendering.
     *
     * @param flag
     */
    void setRenderGrid(bool flag) noexcept
    {
        m_renderGrid = flag;
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
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     * @param color Grid color.
     */
    void draw(const Vector<float>& scale, const Color& color) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void resize(Vector<unsigned int> size) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Grid size.
    Vector<unsigned int> m_size;

    /// If grid shoudl be rendered.
    bool m_renderGrid = false;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
