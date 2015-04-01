
/* ************************************************************************ */

// Declaration
#include "render/GridValue.hpp"

// C++
#include <array>
#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

struct Vertex
{
    GLfloat x, y;
    GLfloat u, v;
};

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

GridValue::GridValue() noexcept
    : Drawable()
{
    // Generate texture
    gl(glGenTextures(1, &m_texture));

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    //gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/* ************************************************************************ */

GridValue::~GridValue()
{
    // Delete program
    gl(glDeleteTextures(1, &m_texture));
}

/* ************************************************************************ */

void GridValue::render(const Vector<float>& scale) noexcept
{
    gl(glPushMatrix());
    gl(glScalef(scale.x, scale.y, 1));

    // Use texture
    gl(glEnable(GL_TEXTURE_2D));
    gl(glBindTexture(GL_TEXTURE_2D, m_texture));

    // Bind buffer
    gl(glBindBuffer(GL_ARRAY_BUFFER, getBuffer()));
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

    gl(glPopMatrix());
}

/* ************************************************************************ */

void GridValue::resize(unsigned int width, unsigned int height, const float* data) noexcept
{
    m_width = width;
    m_height = height;

    std::array<Vertex, 4> vertices = {{
        { 0.5f,  0.5f, 1.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    }};

    // Generate texture
    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    gl(glBindTexture(GL_TEXTURE_2D, m_texture));
    gl(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED, GL_FLOAT, data));

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
