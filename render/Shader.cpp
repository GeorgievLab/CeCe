/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "render/Shader.hpp"

// C++
#include <cassert>

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
#include "core/DynamicArray.hpp"
#include "core/String.hpp"
#include "core/Exception.hpp"
#include "render/errors.hpp"

/* ************************************************************************ */

#ifdef _WIN32
static PFNGLCREATESHADERPROC glCreateShader = nullptr;
static PFNGLDELETESHADERPROC glDeleteShader = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
#endif

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Shader::~Shader()
{
#ifdef _WIN32
	if (!glDeleteShader)
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
#endif

    if (m_id)
        gl(glDeleteShader(m_id));
}

/* ************************************************************************ */

void Shader::init(Type type, const char* source, unsigned length)
{
#ifdef _WIN32
	if (!glCreateShader)
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
#endif

    if (type == Type::VERTEX)
        m_id = glCreateShader(GL_VERTEX_SHADER);
    else if (type == Type::FRAGMENT)
        m_id = glCreateShader(GL_FRAGMENT_SHADER);
    else
        m_id = 0;

    assert(m_id);

#ifdef _WIN32
	if (!glShaderSource)
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
#endif

    // Set shader source
    const GLchar* src = source;
    const GLint len = length;
    gl(glShaderSource(m_id, 1, &source, &len));

#ifdef _WIN32
	if (!glCompileShader)
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
#endif

    // Compile shader
    gl(glCompileShader(m_id));

#ifdef _WIN32
	if (!glGetShaderiv)
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
#endif

    // Check if was compiled
    GLint isCompiled = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

#ifdef _WIN32
		if (!glGetShaderInfoLog)
			glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
#endif

        // The maxLength includes the NULL character
        DynamicArray<GLchar> err(maxLength);
        glGetShaderInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw RuntimeException(String(err.begin(), err.end()));
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */
