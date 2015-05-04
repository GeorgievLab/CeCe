
/* ************************************************************************ */

// Declaration
#include "render/GridColor.hpp"

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

void GridColor::set(const Vector<PositionType>& coord, const Color& color) noexcept
{
    const GLfloat pixels[4] = {color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()};

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, coord.getX(), coord.getY(), 1, 1, GL_RGBA, GL_FLOAT, pixels);
}

/* ************************************************************************ */

void GridColor::init(Context& context)
{
    // Generate texture
    gl(glGenTextures(1, &m_texture));

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Vertex structure
    struct Vertex { GLfloat x, y; GLfloat u, v; };

    const std::array<Vertex, 4> vertices = {{
        { 0.5f,  0.5f, 1.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    }};

    m_buffer.init(context,
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

void GridColor::draw(Context& context) noexcept
{
    // Use texture
    gl(glEnable(GL_TEXTURE_2D));
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_buffer.getId()));
    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0));
    gl(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(2 * sizeof(GLfloat))));

    // Draw circle
    gl(glDrawArrays(GL_QUADS, 0, 4));

    // Disable states
    gl(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    gl(glDisableClientState(GL_VERTEX_ARRAY));
    gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

    gl(glBindTexture(GL_TEXTURE_2D, 0));
    gl(glDisable(GL_TEXTURE_2D));
}

/* ************************************************************************ */

void GridColor::resize(Vector<PositionType> size) noexcept
{
    GridBase::resize(std::move(size));

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().getWidth(), getSize().getHeight(), 0, GL_RGBA, GL_FLOAT, nullptr));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
