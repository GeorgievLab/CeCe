/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/VectorUnits.hpp"

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
    const PositionVector& getPosition() const NOEXCEPT
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
    void setPosition(PositionVector pos) NOEXCEPT
    {
        m_position = std::move(pos);
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
    PositionVector m_position{PositionVector::Zero};

    /// Zoom value.
    float m_zoom = 1.0f;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
