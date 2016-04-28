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

// Declaration
#include "cece/render/PhysicsDebugger.hpp"

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "cece/render/glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

#include <stdio.h>
#include <stdarg.h>

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

void PhysicsDebugger::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    glPushAttrib(GL_CURRENT_BIT);

    glColor3f(color.r, color.g, color.b);

    glBegin(GL_LINES);
    glVertex2f(m_scale * p1.x, m_scale * p1.y);
    glVertex2f(m_scale * p2.x, m_scale * p2.y);
    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

void PhysicsDebugger::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    glPushAttrib(GL_CURRENT_BIT);

    glColor3f(color.r, color.g, color.b);

    glBegin(GL_LINE_LOOP);

    for (int32 i = 0; i < vertexCount; ++i)
        glVertex2f(m_scale * vertices[i].x, m_scale * vertices[i].y);

    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

void PhysicsDebugger::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    glBegin(GL_TRIANGLE_FAN);

    for (int32 i = 0; i < vertexCount; ++i)
        glVertex2f(m_scale * vertices[i].x, m_scale * vertices[i].y);

    glEnd();

    glDisable(GL_BLEND);

    glColor4f(color.r, color.g, color.b, 1.0f);

    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < vertexCount; ++i)
        glVertex2f(m_scale * vertices[i].x, m_scale * vertices[i].y);

    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

void PhysicsDebugger::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    glPushAttrib(GL_CURRENT_BIT);

    const float32 k_segments = 16.0f;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    glColor3f(color.r, color.g, color.b);

    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertex2f(m_scale * v.x, m_scale * v.y);
        theta += k_increment;
    }
    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

void PhysicsDebugger::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);

    const float32 k_segments = 16.0f;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    glBegin(GL_TRIANGLE_FAN);
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertex2f(m_scale * v.x, m_scale * v.y);
        theta += k_increment;
    }
    glEnd();
    glDisable(GL_BLEND);

    theta = 0.0f;
    glColor4f(color.r, color.g, color.b, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertex2f(m_scale * v.x, m_scale * v.y);
        theta += k_increment;
    }
    glEnd();

    b2Vec2 p = center + radius * axis;
    glBegin(GL_LINES);
    glVertex2f(m_scale * center.x, m_scale * center.y);
    glVertex2f(m_scale * p.x, m_scale * p.y);
    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

void PhysicsDebugger::DrawTransform(const b2Transform& xf)
{
    glPushAttrib(GL_CURRENT_BIT);

    b2Vec2 p1 = xf.p, p2;
    //const float32 k_axisScale = 0.4f / m_scale;
    const float32 k_axisScale = 1.0f / m_scale;
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(m_scale * p1.x, m_scale * p1.y);
    p2 = p1 + k_axisScale * xf.q.GetXAxis();
    glVertex2f(m_scale * p2.x, m_scale * p2.y);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(m_scale * p1.x, m_scale * p1.y);
    p2 = p1 + k_axisScale * xf.q.GetYAxis();
    glVertex2f(m_scale * p2.x, m_scale * p2.y);

    glEnd();

    glPopAttrib();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
