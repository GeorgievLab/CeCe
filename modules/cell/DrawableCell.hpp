
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "render/Buffer.hpp"

/* ************************************************************************ */

namespace render { class Context; }

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Rendering object for cell.
 */
class DrawableCell
{

// Public Operations
public:


    /**
     * @brief Initialize.
     *
     * @param context Rendering context.
     */
    void init(render::Context& context);


    /**
     * @brief Render at given position with given rotation.
     *
     * @param context
     */
    void draw(render::Context& context) noexcept;


// Private Data Members
private:

    /// Buffer object.
    render::Buffer m_buffer;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
