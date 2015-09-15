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

# Find OpenCV
if (UNIX AND NOT APPLE)
    # OpenCV 2 (Ubuntu LTS)
    find_package(OpenCV REQUIRED core imgproc highgui)
else ()
    # OpenCV 3
    find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
endif ()

# Include OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# ######################################################################### #

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.cpp
)

# Build plugin
build_plugin(background
    SOURCES ${SRCS}
    LIBRARIES ${OpenCV_LIBRARIES}
)

# ######################################################################### #

if (WIN32)
    set(LIBRARIES "")
    set(COMPONENTS "core;imgproc;highgui;imgcodecs")

    foreach (DIR ${OpenCV_LIB_DIR})
        set(DIR "${DIR}/../bin")

        foreach (COMPONENT ${COMPONENTS})
            file(GLOB FILES "${DIR}/libopencv_${COMPONENT}*.dll")
            set(LIBRARIES ${LIBRARIES} ${FILES})
        endforeach ()
    endforeach ()

    list(REMOVE_DUPLICATES LIBRARIES)

    install(FILES ${LIBRARIES}
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

# ######################################################################### #
