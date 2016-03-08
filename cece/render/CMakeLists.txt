# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

if (!CONFIG_RENDER_ENABLE)
    message(WARNING "Cannot build 'render' without CONFIG_RENDER_ENABLE")
endif ()

# ######################################################################### #

# Options
option(DEV_RENDER_GL_ERRORS "Enable OpenGL errors checking." On)

# ######################################################################### #

# Create project
project(render CXX)

# ######################################################################### #

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
    GridColorColorMap.hpp
    GridColorColorMap.cpp
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

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE On)

# ######################################################################### #

# Install header files
install(DIRECTORY .
    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
    COMPONENT Headers
    FILES_MATCHING PATTERN "*.hpp"
)

# ######################################################################### #
