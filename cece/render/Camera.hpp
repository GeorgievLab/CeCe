/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/VectorUnits.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Render camera.
 */
class CECE_EXPORT Camera
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
    RealType getZoom() const noexcept
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
    void setZoom(RealType zoom) noexcept
    {
        m_zoom = zoom;
    }


// Private Data Members
private:

    /// Camera position.
    PositionVector m_position;

    /// Zoom value.
    RealType m_zoom = 1.0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
