
/* ************************************************************************ */

// Declaration
#include "render/Program.hpp"

// C++
#include <cassert>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Program::Program(const Shader& vs, const Shader& fs) noexcept
{
    m_id = glCreateProgram();
    assert(m_id);

    gl(glAttachShader(m_id, vs.getId()));
    gl(glAttachShader(m_id, fs.getId()));

    gl(glLinkProgram(m_id));
}

/* ************************************************************************ */

Program::~Program()
{
    gl(glDeleteProgram(m_id));
}

/* ************************************************************************ */

}

/* ************************************************************************ */
