
/* ************************************************************************ */

// Declaration
#include "render/Grid.hpp"

// C++
#include <vector>
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Grid::init(Context& context)
{
    m_buffer.init(context);
}

/* ************************************************************************ */

void Grid::draw(const Vector<float>& scale, const Color& color) noexcept
{
    if (!isRenderGrid())
        return;

    // Draw color
    gl(glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));

    gl(glPushMatrix());
    gl(glScalef(scale.getX(), scale.getY(), 1));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    auto width = m_size.getWidth();
    auto height = m_size.getHeight();

    // Draw circle
    gl(glDrawArrays(GL_LINES, 0, 2 * ((width + 1) + (height + 1))));

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glPopMatrix());
}

/* ************************************************************************ */

void Grid::resize(Vector<unsigned int> size) noexcept
{
    m_size = std::move(size);

    auto width = m_size.getWidth();
    auto height = m_size.getHeight();

    constexpr Vector<float> start{-0.5f, -0.5f};
    const Vector<float> step{1.f / width, 1.f / height};

    struct Vertex { GLfloat x, y; };

    std::vector<Vertex> vertices;
    vertices.reserve((width + 1) * (height + 1));

    // X lines
    for (unsigned i = 0; i <= width; ++i)
    {
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY()});
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY() + 1.f});
    }

    // Y lines
    for (unsigned i = 0; i <= height; ++i)
    {
        vertices.push_back(Vertex{start.getX(), start.getY() + i * step.getY()});
        vertices.push_back(Vertex{start.getX() + 1.f, start.getY() + i * step.getY()});
    }

    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */
