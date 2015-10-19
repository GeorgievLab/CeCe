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

# Project
project(cli CXX)

# ######################################################################### #

# Options
if (CONFIG_RENDER_ENABLE)
    option(CONFIG_CLI_ENABLE_IMAGE_CAPTURE "Enable support for image capturing" On)
    option(CONFIG_CLI_ENABLE_VIDEO_CAPTURE "Enable support for video capturing" On)
    set(CONFIG_CLI_VISUALIZE_WIDTH  800 CACHE STRING "Visualization window width")
    set(CONFIG_CLI_VISUALIZE_HEIGHT 600 CACHE STRING "Visualization window height")
endif ()

# ######################################################################### #

# Set some definitions
add_definitions(
    -DAPP_NAME="${APP_NAME}"
    -DDIR_PLUGINS="${PLUGINS_DIRECTORY}"
)

if (CONFIG_RENDER_ENABLE)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIR})

    add_definitions(
        -DCONFIG_CLI_VISUALIZE_WIDTH=${CONFIG_CLI_VISUALIZE_WIDTH}
        -DCONFIG_CLI_VISUALIZE_HEIGHT=${CONFIG_CLI_VISUALIZE_HEIGHT}
    )
endif ()

if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE OR CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
    if (NOT CONFIG_RENDER_ENABLE)
        message(FATAL_ERROR "Video/image capture requires rendering support")
    endif ()

    set(OPENCV_COMPONENTS core)

    if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE)
        set(OPENCV_COMPONENTS ${OPENCV_COMPONENTS} imgproc highgui)
    endif ()

    if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
        # OpenCV 2
        set(OPENCV_COMPONENTS ${OPENCV_COMPONENTS} highgui)
    endif ()

    # Find OpenCV
    find_package(OpenCV 2.4 COMPONENTS ${OPENCV_COMPONENTS})

    # OpenCV not found
    if (OpenCV_FOUND)
        message(STATUS "Video/image capture support: OpenCV ${OpenCV_VERSION}")
        include_directories(${OpenCV_INCLUDE_DIRS})

        if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE)
            add_definitions(-DCONFIG_CLI_ENABLE_IMAGE_CAPTURE=1)
        endif ()

        if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
            add_definitions(-DCONFIG_CLI_ENABLE_VIDEO_CAPTURE=1)
        endif ()
    else ()
        message(WARNING "OpenCV not found, disabling video/image capture")
        set(CONFIG_CLI_ENABLE_VIDEO_CAPTURE Off)
        set(CONFIG_CLI_ENABLE_IMAGE_CAPTURE Off)
    endif ()
endif ()

# ######################################################################### #

# Sources
set(SRCS
    main.cpp
)

# Configure version file
configure_file(version.hpp.in version.hpp @ONLY)
include_directories(${PROJECT_BINARY_DIR})

if (APPLE)
    set(SRCS ${SRCS} icons/${APP_NAME}.icns)
    set_source_files_properties(icons/${APP_NAME}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    configure_file(version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}-${PROJECT_NAME}")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015 Georgiev Lab")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "cz.zcu.ccy.${APP_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "${APP_NAME}.icns")

# ######################################################################### #

# Create executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${SRCS}
)

# ######################################################################### #

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    loader-xml
    loader-reactions
    simulator
)

if (CONFIG_RENDER_ENABLE)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARIES}
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
    target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBRARIES}
    )
endif ()

# ######################################################################### #

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${INSTALL_DIR_BUNDLE}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
    COMPONENT Runtime
)

# Fixup shared libraries for bundle
if (APPLE)
    install(PROGRAMS
        "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.sh"
        DESTINATION "${INSTALL_DIR_RUNTIME}"
        COMPONENT Runtime
    )

    set(APP "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_BUNDLE}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app")
    set(DIRS "/usr/local/lib;\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY};${OSX_LIB_DIRS}")

    install(CODE
        "
        file(GLOB PLUGINS RELATIVE \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}\"
            \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
        )
        include(BundleUtilities)
        fixup_bundle(\"${APP}\" \"\${PLUGINS}\" \"${DIRS}\")
        "
        COMPONENT Runtime
    )
endif ()

if (WIN32 AND (CONFIG_CLI_ENABLE_IMAGE_CAPTURE OR CONFIG_CLI_ENABLE_VIDEO_CAPTURE))
    set(LIBRARIES "")
    set(COMPONENTS ${OPENCV_COMPONENTS})

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
