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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

#ifndef CECE_ENABLE_BOX2D_PHYSICS_DEBUG
#error Physics debugging has to be enabled.
#endif

/* ************************************************************************ */

// Box2D
#include <Box2D/Box2D.h>

// CeCe
#include "cece/export.hpp"

/* ************************************************************************ */

struct b2AABB;

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Physics debugger.
 *
 * @note Based on Erin Catto's version (http://www.box2d.org).
 */
class CECE_EXPORT PhysicsDebugger : public ::b2Draw
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param scale
     */
    PhysicsDebugger(float32 scale = 1.0) noexcept
        : m_scale(scale)
    {
        // Nothing to do
    }


// Public Mutators
public:


    /**
     * @brief Change render scale.
     *
     * @param scale
     */
    void setScale(float32 scale) noexcept
    {
        m_scale = scale;
    }


// Public Operations
public:


    /**
     * @brief Draw a line segment.
     *
     * @param p1
     * @param p2
     * @param color
     */
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;


    /**
     * @brief Draw a closed polygon provided in CCW order.
     *
     * @param vertices
     * @param vertexCount
     * @param color
     */
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;


    /**
     * @brief Draw a solid closed polygon provided in CCW order.
     *
     * @param vertices
     * @param vertexCount
     * @param color
     */
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;


    /**
     * @brief Draw a circle.
     *
     * @param center
     * @param radius
     * @param color
     */
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

    /**
     * @brief Draw a solid circle.
     *
     * @param center
     * @param radius
     * @param axis
     * @param color
     */
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;


    /**
     * @brief Draw a transform. Choose your own length scale.
     *
     * @param xf
     */
    void DrawTransform(const b2Transform& xf) override;


// Private Data Members
private:

    /// Render scale.
    float32 m_scale;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
