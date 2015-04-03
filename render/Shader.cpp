
/* ************************************************************************ */

// Declaration
#include "render/Shader.hpp"

// C++
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>

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

    // Check if was compiled
    GLint isCompiled = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> err(maxLength);
        glGetShaderInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw std::runtime_error(std::string(err.begin(), err.end()));
    }
}

/* ************************************************************************ */

Shader::~Shader()
{
    gl(glDeleteShader(m_id));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
