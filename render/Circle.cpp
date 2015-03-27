
/* ************************************************************************ */

// Declaration
#include "render/Circle.hpp"

// C++
#include <cmath>
#include <array>

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Circle::Circle() noexcept
    : Drawable()
{
#if RENDER_USE_VBO
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

    glBindBuffer(GL_ARRAY_BUFFER, getBuffer());
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    );
#endif
}

/* ************************************************************************ */

void Circle::render() noexcept
{
#if RENDER_USE_VBO
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, getBuffer());
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);

    // Draw circle
    glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS + 2);

    // Disable states
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);

    constexpr unsigned SEGMENTS = 50;
    constexpr float STEP = 2 * 3.14159265359 / SEGMENTS;

    for (unsigned n = 0; n <= SEGMENTS; ++n)
    {
        float alpha = STEP * n;
        glVertex2f(sin(alpha) * radius, cos(alpha) * radius);
    }

    glEnd();
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
