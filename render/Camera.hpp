
#pragma once

/* ************************************************************************ */

// Physics
#include "render/Position.hpp"

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Render camera.
 */
class DLL_EXPORT Camera
{

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
    const Position& getPosition() const NOEXCEPT
    {
        return m_position;
    }


    /**
     * @brief Returns camera zoom.
     *
     * @return
     */
    float getZoom() const NOEXCEPT
    {
        return m_zoom;
    }


// Public Mutators
public:


    /**
     * @brief Change camera position.
     *
     * @param pos
     */
    void setPosition(const Position& pos) NOEXCEPT
    {
        m_position = pos;
    }


    /**
     * @brief Set camera zoom.
     *
     * @param zoom
     */
    void setZoom(float zoom) NOEXCEPT
    {
        m_zoom = zoom;
    }


// Private Data Members
private:

    /// Camera position.
    Position m_position{0, 0};

    /// Zoom value.
    float m_zoom = 1.0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
