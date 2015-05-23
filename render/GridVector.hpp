
#pragma once

/* ************************************************************************ */

// Simulator
#include "render/Position.hpp"
#include "render/Color.hpp"
#include "render/Buffer.hpp"
#include "render/Shader.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief OpenGL grid for vectors object.
 */
class GridVector
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    GridVector(Context& context, Vector<unsigned int> size, const Vector<float>* data);


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
     * @brief If velocities rendering is enabled.
     *
     * @return
     */
    bool isRenderVelocity() const noexcept
    {
        return m_renderVelocity;
    }


// Public Mutators
public:


    /**
     * @brief Enable or disable velocities rendering.
     *
     * @param flag
     */
    void setRenderVelocity(bool flag) noexcept
    {
        m_renderVelocity = flag;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param scale Grid scale.
     */
    void draw(const Vector<float>& scale) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(Vector<unsigned int> size, const Vector<float>* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const Vector<float>* data) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Grid size.
    Vector<unsigned int> m_size;

    /// If velocities should be rendered.
    bool m_renderVelocity = false;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
