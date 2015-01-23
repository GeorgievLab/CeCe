
#pragma once

/* ************************************************************************ */

// Core
#include "core/Units.h"

// Physics
#include "physics/Position.h"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Render camera.
 */
class Camera
{

// Public Enums
public:


    /**
     * @brief Camera modes.
     */
    enum class Mode
    {
        Top,
        Isometric
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Camera() = default;


// Public Accessors
public:


    /**
     * @brief Get camera position.
     *
     * @return
     */
    const Position& getPosition() const noexcept
    {
        return m_position;
    }


    /**
     * @brief Returns camera mode.
     *
     * @return
     */
    Mode getMode() const noexcept
    {
        return m_mode;
    }


    /**
     * @brief Returns camera zoom.
     *
     * @return
     */
    float getZoom() const noexcept
    {
        return m_zoom;
    }


// Public Mutators
public:


    /**
     * @brief Set camera mode.
     *
     * @param mode
     */
    void setMode(Mode mode) noexcept
    {
        m_mode = mode;
    }


    /**
     * @brief Change camera position.
     *
     * @param pos
     */
    void setPosition(const Position& pos) noexcept
    {
        m_position = pos;
    }


    /**
     * @brief Set camera zoom.
     *
     * @param zoom
     */
    void setZoom(float zoom) noexcept
    {
        m_zoom = zoom;
    }


// Private Data Members
private:

    /// Camera mode.
    Mode m_mode = Mode::Top;

    /// Camera position.
    Position m_position{};

    /// Zoom value.
    float m_zoom = 1.0;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
