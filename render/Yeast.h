
#ifndef _RENDER_YEAST_H_
#define _RENDER_YEAST_H_

/* ************************************************************************ */

// Core
#include "core/Units.h"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Renderer for yeast.
 */
class Yeast
{

// Public Operations
public:

    /**
     * @brief Render yeast.
     *
     * @param context
     * @param x
     * @param y
     * @param radius
     */
    static void render(Context& context, MicroMeters x, MicroMeters y, MicroMeters radius) noexcept;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _RENDER_YEAST_H_
