
#pragma once

/* ************************************************************************ */

// Physics
#include "render/Position.hpp"

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


    /**
     * @brief Returns camera rotation.
     *
     * @return
     */
    float getRotation() const noexcept
    {
        return m_rotation;
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


    /**
     * @brief Set camera rotation.
     *
     * @param rotation
     */
    void setRotation(float rotation) noexcept
    {
        m_rotation = rotation;
    }


// Private Data Members
private:

    /// Camera mode.
    Mode m_mode = Mode::Top;

    /// Camera position.
    Position m_position{};

    /// Zoom value.
    float m_zoom = 1.0;

    /// Rotation.
    float m_rotation = 0.f;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
