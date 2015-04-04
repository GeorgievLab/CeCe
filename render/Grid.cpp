
/* ************************************************************************ */

// Declaration
#include "render/Grid.hpp"

// C++
#include <vector>
#include <cassert>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Grid::render(const Vector<float>& scale, const Color& color) noexcept
{
    // Draw color
    gl(glColor4f(color.red, color.green, color.blue, color.alpha));

    gl(glPushMatrix());
    gl(glScalef(scale.getX(), scale.getY(), 1));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    // Draw circle
    gl(glDrawArrays(GL_LINES, 0, 2 * ((m_width + 1) + (m_height + 1))));

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

void Grid::resize(unsigned int width, unsigned int height) noexcept
{
    m_width = width;
    m_height = height;

    constexpr Vector<float> start{-0.5f, -0.5f};
    const Vector<float> step{1.f / m_width, 1.f / m_height};

    struct Vertex { GLfloat x, y; };

    std::vector<Vertex> vertices;
    vertices.reserve((m_width + 1) * (m_height + 1));

    // X lines
    for (unsigned i = 0; i <= m_width; ++i)
    {
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY()});
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY() + 1.f});
    }

    // Y lines
    for (unsigned i = 0; i <= m_height; ++i)
    {
        vertices.push_back(Vertex{start.getX(), start.getY() + i * step.getY()});
        vertices.push_back(Vertex{start.getX() + 1.f, start.getY() + i * step.getY()});
    }

    assert(getBuffer() != 0);

    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    ));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
