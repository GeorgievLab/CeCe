
/* ************************************************************************ */

// Declaration
#include "render/GridVector.hpp"

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
    GLfloat red, green, blue;
};

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void GridVector::render(const Vector<float>& scale) noexcept
{
    gl(glPushMatrix());
    gl(glScalef(scale.getX(), scale.getY(), 1));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glEnableClientState(GL_COLOR_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0));
    gl(glColorPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(2 * sizeof(GLfloat))));

    // Draw circle
    gl(glDrawArrays(GL_LINES, 0, 2 * m_width * m_height));

    // Disable states
    gl(glDisableClientState(GL_COLOR_ARRAY));
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

void GridVector::resize(unsigned int width, unsigned int height, const Vector<float>* data) noexcept
{
    m_width = width;
    m_height = height;

    constexpr Vector<float> start{-0.5f, -0.5f};
    const Vector<float> step{1.f / m_width, 1.f / m_height};

    std::vector<Vertex> vertices;
    vertices.reserve(2 * m_width * m_height);

    // Get maximum value
    float max_squared = 1.f;
    for (decltype(m_width) i = 0; i < m_width; ++i)
    {
        for (decltype(m_height) j = 0; j < m_height; ++j)
        {
            const Vector<float>& vec = data[i + j * m_width];
            const float len_squared = vec.getLengthSquared();

            if (max_squared < len_squared)
                max_squared = len_squared;
        }
    }

    // Maximum value
    const float max = std::sqrt(max_squared);

    // Draw grid vectors
    for (decltype(m_width) i = 0; i < m_width; ++i)
    {
        for (decltype(m_height) j = 0; j < m_height; ++j)
        {
            // Get vector normalized by max length
            const Vector<float> vec = data[i + j * m_width] / max;
            const Vector<float> pos{
                    start.getX() + i * step.getX() + step.getX() / 2.f,
                    start.getY() + j * step.getY() + step.getY() / 2.f
            };
            const float red = 5 * fabs(vec.getX());
            const float green = 5 * std::max(vec.getY(), 0.f);
            const float blue = 5 * std::max(-vec.getY(), 0.f);

            const Vector<float> dest = pos + vec * step;

            vertices.push_back(Vertex{pos.getX(), pos.getY(), red, green, blue});
            vertices.push_back(Vertex{dest.getX(), dest.getY(), red, green, blue});
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
