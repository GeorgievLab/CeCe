
/* ************************************************************************ */

// Declaration
#include "render/Shader.hpp"

// C++
#include <cassert>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Shader::Shader(Type type, const char* source, unsigned length) noexcept
{
    if (type == Type::VERTEX)
        m_id = glCreateShader(GL_VERTEX_SHADER);
    else if (type == Type::FRAGMENT)
        m_id = glCreateShader(GL_FRAGMENT_SHADER);
    else
        m_id = 0;

    assert(m_id);

    // Set shader source
    const GLchar* src = source;
    const GLint len = length;
    gl(glShaderSource(m_id, 1, &source, &len));

    // Compile shader
    gl(glCompileShader(m_id));
}

/* ************************************************************************ */

Shader::~Shader()
{
    gl(glDeleteShader(m_id));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
