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
    message(FATAL_ERROR "Cannot build module without CONFIG_RENDER_ENABLE")
endif ()

# LibPNG is required
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

# ######################################################################### #

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Plugin.cpp
)

# Build plugin
build_plugin(picture
    SOURCES ${SRCS}
    LIBRARIES ${PNG_LIBRARIES}
)

# ######################################################################### #

if (WIN32)
    # Install PNG libraries
    install(FILES ${PNG_LIBRARIES}
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

# ######################################################################### #
