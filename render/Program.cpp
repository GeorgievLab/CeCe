
/* ************************************************************************ */

// Declaration
#include "render/Program.hpp"

// C++
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Program::~Program()
{
    if (m_id)
        gl(glDeleteProgram(m_id));
}

/* ************************************************************************ */

Program::UniformId Program::getUniformId(const char* name) const noexcept
{
    assert(getId());
    Program::UniformId id;
    gl(id = glGetUniformLocation(getId(), name));
    return id;
}

/* ************************************************************************ */

void Program::init(const Shader& vs, const Shader& fs)
{
    m_id = glCreateProgram();
    assert(m_id);

    gl(glAttachShader(m_id, vs.getId()));
    gl(glAttachShader(m_id, fs.getId()));

    gl(glLinkProgram(m_id));

    GLint isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> err(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw std::runtime_error(std::string(err.begin(), err.end()));
    }

    glDetachShader(m_id, vs.getId());
    glDetachShader(m_id, fs.getId());
}

/* ************************************************************************ */

}

/* ************************************************************************ */
