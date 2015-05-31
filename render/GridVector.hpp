
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Vector.hpp"
#include "render/Context.hpp"
#include "render/Buffer.hpp"
#include "render/GridBase.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief OpenGL grid for vectors object.
 */
class DLL_EXPORT GridVector : public GridBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    GridVector(Context& context, core::Vector<unsigned int> size, const core::Vector<float>* data);


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     */
    void draw(Context& context) NOEXCEPT;


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

};

/* ************************************************************************ */

}

/* ************************************************************************ */
