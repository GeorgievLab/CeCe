
/* ************************************************************************ */

// Declaration
#include "render/Buffer.hpp"

// C++
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Buffer::Buffer(Context& context)
{
    // Generate buffer
    gl(glGenBuffers(1, &m_id));
    assert(m_id != 0);
}

/* ************************************************************************ */

Buffer::Buffer(Context& context, SizeType size, const void* data)
    : Buffer(context)
{
    // Init data
    resize(size, data);
}

/* ************************************************************************ */

Buffer::~Buffer()
{
    assert(isInitialized());
    gl(glDeleteBuffers(1, &m_id));
}

/* ************************************************************************ */

void Buffer::resize(SizeType size, const void* data)
{
    assert(isInitialized());

    // Bind buffer and set data
    gl(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    gl(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
