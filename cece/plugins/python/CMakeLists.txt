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

if (WIN32)
    set(PYTHON_LIB_PATH "C:/Windows/SysWOW64/python27.dll" CACHE STRING "Path to python2 library")
endif ()

# ######################################################################### #

# Include Python API
find_package(PythonLibs 2.7 REQUIRED)

# Include directories
include_directories(${PYTHON_INCLUDE_DIRS})

# ######################################################################### #

# Sources
set(SRCS
    Handle.hpp
    Handle.cpp
    View.hpp
    View.cpp
    Exception.hpp
    Exception.cpp
    ValueCast.hpp
    Source.hpp
    Source.cpp
    Module.hpp
    Module.cpp
    Program.hpp
    Program.cpp
    Initializer.hpp
    Initializer.cpp
    Object.hpp
    Object.cpp
    Plugin.cpp
    Type.hpp
    Type.cpp

    wrappers/stdout.cpp

    wrappers/core.cpp
    wrappers/core_functions.cpp
    wrappers/core_VectorInt.cpp
    wrappers/core_VectorUint.cpp
    wrappers/core_VectorFloat.cpp
    wrappers/core_VectorPosition.cpp
    wrappers/core_VectorVelocity.cpp
    wrappers/core_VectorForce.cpp
    wrappers/core_VectorAcceleration.cpp

    wrappers/simulator.cpp
    wrappers/simulator_Object.cpp
    wrappers/simulator_Module.cpp
    wrappers/simulator_Simulation.cpp
    wrappers/simulator_Configuration.cpp
)

# Build plugin
build_plugin(python
    SOURCES ${SRCS}
    LIBRARIES ${PYTHON_LIBRARIES}
)

# ######################################################################### #

if (WIN32 AND PYTHON_LIB_PATH)
    # Install examples
    install(FILES ${PYTHON_LIB_PATH}
        DESTINATION "${INSTALL_DIR_LIBRARY}"
    )
endif ()

# ######################################################################### #
