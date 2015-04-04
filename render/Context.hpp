
#pragma once

#ifdef ENABLE_RENDER

/* ************************************************************************ */

// C++
#include <vector>
#include <string>
#include <cstdint>

// Simulator
#include "core/Vector.hpp"
#include "render/Camera.hpp"
#include "render/Position.hpp"
#include "render/Color.hpp"

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
     * @brief Returns if context is init.
     *
     * @return
     */
    bool isInit() noexcept
    {
        return m_is_init;
    }



    /**
     * @brief Returns context camera.
     *
     * @return
     */
    Camera& getCamera() noexcept
    {
        return m_camera;
    }


    /**
     * @brief Get current image data.
     *
     * @return
     */
    std::vector<std::uint8_t> getData() const noexcept;


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
     * @brief Draw line.
     *
     * @param pos   Origin.
     * @param dir   Direction.
     * @param color Color.
     */
    void drawLine(const Position& pos, const Vector<float>& dir, const Color& color) noexcept;


    /**
     * @brief Draw grid.
     *
     * @param size   Grid size.
     * @param count  Number of parts.
     * @param color  Grid color.
     */
    void drawGrid(const Vector<float>& size, const Vector<unsigned>& count, const Color& color) noexcept;


    /**
     * @brief Draw 2d text.
     *
     * @param text
     * @param x
     * @param y
     */
    void drawText(const std::string& text, float x, float y);


// Private Data Members
private:

    /// If context is init
    bool m_is_init = false;

    /// Camera.
    Camera m_camera;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif

/* ************************************************************************ */
