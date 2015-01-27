
#pragma once

/* ************************************************************************ */

// Render
#include "render/Camera.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;
class Cell;
class Barrier;

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Renderer context.
 */
class Context
{

// Public Accessors
public:


    /**
     * @brief Returns context camera.
     *
     * @return
     */
    Camera& getCamera() noexcept
    {
        return m_camera;
    }


// Public Operations
public:


    /**
     * @brief Initialize context.
     */
    void init() noexcept;


    /**
     * @brief Setup camera view.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setView(int width, int height) noexcept;


    /**
     * @brief Begin frame rendering.
     *
     * @param width  View width.
     * @param height View height.
     */
    void frameBegin(int width, int height) noexcept;


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
     * @brief Draw cell with sphere shape.
     *
     * @param cell
     */
    void drawCellSphere(const simulator::Cell& cell) noexcept;


    /**
     * @brief Draw barrier.
     *
     * @param barrier
     */
    void drawBarrier(const simulator::Barrier& barrier) noexcept;


// Private Data Members
private:

    /// Camera.
    Camera m_camera;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

