
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
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
	GridVector(Context& context, core::Vector<unsigned int> size, const core::Vector<float>* data);


// Public Accessors
public:


    /**
     * @brief Returns current grid size.
     *
     * @return
     */
    const core::Vector<unsigned int>& getSize() const NOEXCEPT
    {
        return m_size;
    }


    /**
     * @brief If velocities rendering is enabled.
     *
     * @return
     */
    bool isRenderVelocity() const NOEXCEPT
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
    void setRenderVelocity(bool flag) NOEXCEPT
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
	void draw(const core::Vector<float>& scale) NOEXCEPT;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
	void resize(core::Vector<unsigned int> size, const core::Vector<float>* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
	void update(const core::Vector<float>* data) NOEXCEPT;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Grid size.
	core::Vector<unsigned int> m_size;

    /// If velocities should be rendered.
    bool m_renderVelocity = false;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
