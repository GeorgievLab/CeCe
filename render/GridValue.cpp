
/* ************************************************************************ */

// Declaration
#include "render/GridValue.hpp"

// C++
#include <vector>
#include <cassert>
#include <cmath>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat red, green, blue;//, alpha;
};

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void GridValue::render(const Vector<float>& scale) noexcept
{
    gl(glPushMatrix());
    gl(glScalef(scale.x, scale.y, 1));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glEnableClientState(GL_COLOR_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0));
    gl(glColorPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(2 * sizeof(GLfloat))));

    // Draw circle
    gl(glDrawArrays(GL_QUADS, 0, 4 * m_width * m_height));

    // Disable states
    gl(glDisableClientState(GL_COLOR_ARRAY));
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

void GridValue::resize(unsigned int width, unsigned int height, const unsigned char* data) noexcept
{
    m_width = width;
    m_height = height;

    constexpr Vector<float> start{-0.5f, -0.5f};
    const Vector<float> step{1.f / m_width, 1.f / m_height};

    std::vector<Vertex> vertices;
    vertices.reserve(4 * m_width * m_height);

    // Draw grid vectors
    for (decltype(m_width) i = 0; i < m_width; ++i)
    {
        for (decltype(m_height) j = 0; j < m_height; ++j)
        {
            // Get vector normalized by max length
            const unsigned value = data[i + j * m_width];
            const Vector<float> pos{start.x + i * step.x, start.y + j * step.y};
            const float red = 1 - (value / 256.f);
            const float green = 1 - (value / 256.f);
            const float blue = 1;
            const float alpha = 1.0f;//value / 256.f;

            vertices.push_back(Vertex{pos.x, pos.y, red, green, blue});
            vertices.push_back(Vertex{pos.x, pos.y + step.y, red, green, blue});
            vertices.push_back(Vertex{pos.x + step.x, pos.y + step.y, red, green, blue});
            vertices.push_back(Vertex{pos.x + step.x, pos.y, red, green, blue});
        }
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
