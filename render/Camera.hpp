/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/VectorUnits.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Render camera.
 */
class Camera
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
    const PositionVector& getPosition() const noexcept
    {
        return m_position;
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
     * @brief Change camera position.
     *
     * @param pos
     */
    void setPosition(PositionVector pos) noexcept
    {
        m_position = std::move(pos);
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

    /// Camera position.
    PositionVector m_position;

    /// Zoom value.
    float m_zoom = 1.0f;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
