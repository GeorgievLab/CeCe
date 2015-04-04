
/* ************************************************************************ */

// Declaration
#include "render/Circle.hpp"

// C++
#include <cmath>
#include <array>
#include <cassert>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Circle::init()
{
    Drawable::init();

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

    assert(getBuffer() != 0);

    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    ));
}

/* ************************************************************************ */

void Circle::render(const Position& pos, float radius, const Color& color) noexcept
{
    assert(getBuffer() != 0);

    // Draw color
    gl(glColor4f(color.red, color.green, color.blue, color.alpha));

    gl(glPushMatrix());
    gl(glTranslatef(pos.getX(), pos.getY(), 0));
    gl(glScalef(radius, radius, radius));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
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
