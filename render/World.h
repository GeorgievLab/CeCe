
#ifndef _RENDER_WORLD_H_
#define _RENDER_WORLD_H_

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Renderer for yeast.
 */
class World
{

// Public Operations
public:

    /**
     * @brief Render world.
     *
     * @param context
     */
    static void render(Context& context) noexcept;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _RENDER_WORLD_H_
