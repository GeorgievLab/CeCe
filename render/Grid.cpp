
/* ************************************************************************ */

// Declaration
#include "render/Grid.hpp"

// C++
#include <vector>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "render/glext.h"
#pragma comment(lib, "opengl32.lib")
#elif __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#endif

// Simulator
#include "render/Context.hpp"
#include "render/errors.hpp" // TODO: remove

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Grid::Grid(Context& context)
    : m_buffer(context)
{
    // Nothing to do
}

/* ************************************************************************ */

void Grid::draw(Context& context) NOEXCEPT
{
    // Set vertex buffer
    context.setVertexBuffer(&m_buffer);

    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    auto width = getSize().getWidth();
    auto height = getSize().getHeight();

    // Draw grid
    context.draw(PrimitiveType::Lines, 2 * ((width + 1) + (height + 1)));

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));

    // Unbind vertex buffer
    //context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void Grid::resize(core::Vector<PositionType> size) NOEXCEPT
{
    GridBase::resize(std::move(size));

    // Get grid size
    const auto width = getSize().getWidth();
    const auto height = getSize().getHeight();

    CONSTEXPR core::Vector<float> start{-0.5f};
    const core::Vector<float> step = 1.f / getSize();

    struct Vertex { GLfloat x, y; };

    std::vector<Vertex> vertices;
    vertices.reserve((width + 1) * (height + 1));

    // X lines
    for (PositionType i = 0; i <= width; ++i)
    {
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY()});
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY() + 1.f});
    }

    // Y lines
    for (PositionType i = 0; i <= height; ++i)
    {
        vertices.push_back(Vertex{start.getX(), start.getY() + i * step.getY()});
        vertices.push_back(Vertex{start.getX() + 1.f, start.getY() + i * step.getY()});
    }

    // Initialize buffer
    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */
