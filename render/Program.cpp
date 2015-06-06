
/* ************************************************************************ */

// Declaration
#include "render/Program.hpp"

// C++
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>

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
#include "render/errors.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
static PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC glAttachShader = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
static PFNGLDETACHSHADERPROC glDetachShader = nullptr;
#endif

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Program::~Program()
{
#ifdef _WIN32
	if (!glDeleteProgram)
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
#endif

    if (m_id)
        gl(glDeleteProgram(m_id));
}

/* ************************************************************************ */

Program::UniformId Program::getUniformId(const char* name) const NOEXCEPT
{
#ifdef _WIN32
	if (!glGetUniformLocation)
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
#endif

    assert(getId());
    Program::UniformId id;
    gl(id = glGetUniformLocation(getId(), name));
    return id;
}

/* ************************************************************************ */

void Program::init(const Shader& vs, const Shader& fs)
{
#ifdef _WIN32
	if (!glCreateProgram)
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
#endif

    m_id = glCreateProgram();
    assert(m_id);

#ifdef _WIN32
	if (!glAttachShader)
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
#endif

    gl(glAttachShader(m_id, vs.getId()));
    gl(glAttachShader(m_id, fs.getId()));

#ifdef _WIN32
	if (!glLinkProgram)
		glLinkProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glLinkProgram");
#endif

    gl(glLinkProgram(m_id));

#ifdef _WIN32
	if (!glGetProgramiv)
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
#endif

    GLint isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

#ifdef _WIN32
		if (!glGetProgramInfoLog)
			glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
#endif

        // The maxLength includes the NULL character
        std::vector<GLchar> err(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw std::runtime_error(std::string(err.begin(), err.end()));
    }

#ifdef _WIN32
	if (!glDetachShader)
		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
#endif

    glDetachShader(m_id, vs.getId());
    glDetachShader(m_id, fs.getId());
}

/* ************************************************************************ */

}

/* ************************************************************************ */
