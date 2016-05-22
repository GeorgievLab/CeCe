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

# Sources
set(SOURCES
    render/datatype.hpp
    render/Color.hpp
    render/Color.cpp
    render/errors.hpp
    render/errors.cpp
    render/Context.hpp
    render/Context.cpp
    render/Object.hpp
    render/Object.cpp
    render/Camera.hpp
    render/Camera.cpp
    render/Buffer.hpp
    render/Buffer.cpp
    render/GridBase.hpp
    render/Grid.hpp
    render/Grid.cpp
    render/GridVector.hpp
    render/GridVector.cpp
    render/GridColor.hpp
    render/GridColor.cpp
    render/GridColorSmooth.hpp
    render/GridColorSmooth.cpp
    render/GridColorColorMap.hpp
    render/GridColorColorMap.cpp
    render/Shader.hpp
    render/Shader.cpp
    render/Program.hpp
    render/Program.cpp
    render/VertexElement.hpp
    render/VertexFormat.hpp
    render/Texture.hpp
    render/Texture.cpp
    render/Circle.hpp
    render/Circle.cpp
    render/Rectangle.hpp
    render/Rectangle.cpp
    render/ImageData.hpp
    render/Lines.hpp
    render/Lines.cpp
    render/glext.h
)

if (DEV_PHYSICS_BUILTIN_DEBUG)
    set(SOURCES ${SOURCES}
        render/PhysicsDebugger.hpp
        render/PhysicsDebugger.cpp
    )
endif ()

set(SOURCES_RENDER ${SOURCES} PARENT_SCOPE)

# ######################################################################### #
