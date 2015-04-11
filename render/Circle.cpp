
/* ************************************************************************ */

// Declaration
#include "render/Circle.hpp"

// C++
#include <cmath>
#include <array>
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Circle::init()
{
    m_buffer.init();

    constexpr float STEP = 2 * 3.14159265359 / SEGMENTS;

    struct Vertex { GLfloat x, y; };

    std::array<Vertex, SEGMENTS + 2> vertices;
    vertices[0] = Vertex{0.f, 0.f};

    for (unsigned n = 1; n <= SEGMENTS + 1; ++n)
    {
        const float alpha = STEP * n;
        vertices[n] = Vertex{(float) sin(alpha), (float) cos(alpha)};
    }

    vertices[SEGMENTS + 1] = vertices[1];

    assert(m_buffer.getId() != 0);

    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    ));
}

/* ************************************************************************ */

void Circle::draw(const Position& pos, float radius, const Color& color) noexcept
{
    assert(m_buffer.getId() != 0);

    // Draw color
    gl(glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));

    gl(glPushMatrix());
    gl(glTranslatef(pos.getX(), pos.getY(), 0));
    gl(glScalef(radius, radius, radius));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    // Draw circle
    gl(glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2));

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

}

/* ************************************************************************ */
