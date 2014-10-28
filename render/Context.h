
#ifndef _RENDER_CONTEXT_H_
#define _RENDER_CONTEXT_H_

/* ************************************************************************ */
/* FORWARD DECLARATIONS                                                     */
/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;
class Cell;
class Yeast;

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */
/* CLASSES                                                                  */
/* ************************************************************************ */

/**
 * @brief Renderer context.
 */
class Context
{

// Public Operations
public:


    /**
     * @brief Initialize context.
     */
    void init() noexcept;


    /**
     * @brief Setup view from top.
     *
     * @param width  View width.
     * @param height View height.
     * @param zoom   View zoom.
     */
    void setTopView(int width, int height, float zoom) noexcept;


    /**
     * @brief Setup isometric view.
     *
     * @param width  View width.
     * @param height View height.
     * @param zoom   View zoom.
     */
    void setIsometricView(int width, int height, float zoom) noexcept;


    /**
     * @brief Begin frame rendering.
     */
    void frameBegin() noexcept;


    /**
     * @brief End frame rendering.
     */
    void frameEnd() noexcept;


    /**
     * @brief Draw world.
     *
     * @param world
     */
    void drawWorld(const simulator::World& world) noexcept;


    /**
     * @brief Draw cell.
     *
     * @param cell
     */
    void drawCell(const simulator::Cell& cell) noexcept;


    /**
     * @brief Draw yeast.
     *
     * @param yeast
     */
    void drawYeast(const simulator::Yeast& yeast) noexcept;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _RENDER_CONTEXT_H_
