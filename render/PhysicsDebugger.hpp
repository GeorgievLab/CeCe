/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

#ifndef ENABLE_PHYSICS_DEBUG
#error ENABLE_PHYSICS_DEBUG macro required
#endif

/* ************************************************************************ */

// Box2D
#include <Box2D/Box2D.h>

/* ************************************************************************ */

struct b2AABB;

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Physics debugger.
 *
 * @note Based on Erin Catto's version (http://www.box2d.org).
 */
class PhysicsDebugger : public ::b2Draw
{

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

};

/* ************************************************************************ */

}

/* ************************************************************************ */
