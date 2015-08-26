# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (!DEV_PLUGIN_streamlines_RENDER)
    message(DEV_PLUGIN_streamlines_RENDER "Cannot build 'render' without DEV_PLUGIN_streamlines_RENDER")
endif ()

# ######################################################################### #

# Options
option(DEV_RENDER_GL_ERRORS "Enable OpenGL errors checking." On)

# ######################################################################### #

# Create project
project(render CXX)

# ######################################################################### #

# OpenGL library is required
find_package(OpenGL REQUIRED)

# Include OpenGL path
include_directories(
    ${OPENGL_INCLUDE_DIR}
)

# ######################################################################### #

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

# Enable errors
if (DEV_RENDER_GL_ERRORS)
    add_definitions(-DDEV_RENDER_GL_ERRORS=1)
endif ()

# ######################################################################### #

# Sources
set(SRCS
    datatype.hpp
    Color.hpp
    Color.cpp
    errors.hpp
    errors.cpp
    Context.hpp
    Context.cpp
    Object.hpp
    Object.cpp
    Camera.hpp
    Camera.cpp
    Buffer.hpp
    Buffer.cpp
    GridBase.hpp
    Grid.hpp
    Grid.cpp
    GridVector.hpp
    GridVector.cpp
    GridColor.hpp
    GridColor.cpp
    GridColorSmooth.hpp
    GridColorSmooth.cpp
    Shader.hpp
    Shader.cpp
    Program.hpp
    Program.cpp
    VertexElement.hpp
    VertexFormat.hpp
    Texture.hpp
    Texture.cpp
    Circle.hpp
    Circle.cpp
    Rectangle.hpp
    Rectangle.cpp
    ImageData.hpp
    Lines.hpp
    Lines.cpp
    glext.h
)

if (DEV_PHYSICS_BUILTIN_DEBUG)
    set(SRCS ${SRCS}
        PhysicsDebugger.hpp
        PhysicsDebugger.cpp
    )
endif ()

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME} OBJECT ${SRCS})

# ######################################################################### #
