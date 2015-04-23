
/* ************************************************************************ */

// Declaration
#include "DrawableCell.hpp"

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

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Number of circle segments
 */
static constexpr unsigned SEGMENTS = 100;

/* ************************************************************************ */

void DrawableCell::init(render::Context& context)
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

void DrawableCell::draw(render::Context& context, const PositionVector& pos, units::Length radius, units::Angle rotation) noexcept
{
    assert(m_buffer.getId() != 0);

    // Draw color
    gl(glColor4f(1, 1, 1, 1));

    gl(glPushMatrix());
    gl(glTranslatef(pos.getX(), pos.getY(), 0));
    gl(glScalef(radius, radius, radius));
    gl(glRotatef(units::rad2deg(rotation), 0, 0, 1));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    // Draw circle
    gl(glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2));

    glColor4f(1, 0, 0, 1);

    glBegin(GL_LINES);
    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, 1.f);
    glEnd();

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
