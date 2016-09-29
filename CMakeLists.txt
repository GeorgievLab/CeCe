# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
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

cmake_minimum_required(VERSION 3.1)

# ######################################################################### #

set(CECE_VERSION_MAJOR 0)
set(CECE_VERSION_MINOR 6)
set(CECE_VERSION_PATCH 0)
set(CECE_VERSION ${CECE_VERSION_MAJOR}.${CECE_VERSION_MINOR}.${CECE_VERSION_PATCH})

# ######################################################################### #

option(CECE_BUILD_CLI "Build CLI application" On)

# ######################################################################### #

add_subdirectory(core)
add_subdirectory(plugins)

if (CECE_BUILD_CLI)
    add_subdirectory(app/cli)
endif ()

# ######################################################################### #

install(
    DIRECTORY examples/
    DESTINATION examples
    PATTERN ".git" EXCLUDE
)

# ######################################################################### #

# Build unit tests
# CECE_TESTS_BUILD is an option in CeCe-core
if (CECE_TESTS_BUILD)
    enable_testing()
endif ()

# ######################################################################### #

# Configure file for travis-ci -> bintray deployment
if (UNIX AND NOT APPLE)
    set(BINTRAY_PLATFORM "linux-x64")
elseif (APPLE)
    set(BINTRAY_PLATFORM "macOS")
endif ()

set(BINTRAY_VERSION ${CECE_VERSION})

configure_file(resources/descriptor.json.in descriptor.json @ONLY)

# ######################################################################### #

include(InstallRequiredSystemLibraries)

set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
if (${SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_CL_64)
        set(SYSTEM_NAME "Win64")
    else ()
        set(SYSTEM_NAME "Win32")
    endif ()
endif ()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CeCe simulator")
set(CPACK_PACKAGE_VENDOR "Georgiev Lab")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_NAME CeCe)
set(CPACK_PACKAGE_VERSION "${CECE_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR ${CECE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CECE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CECE_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Jiří Fatka <fatkaj@ntis.zcu.cz>")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${SYSTEM_NAME}")

# ######################################################################### #

set(CPACK_STRIP_FILES "${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}")

# ######################################################################### #

include(CPack)

# ######################################################################### #

