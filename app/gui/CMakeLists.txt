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

# Project name
project(gui CXX)

# ######################################################################### #

# Set some definitions
add_definitions(
    -DAPP_NAME="${APP_NAME}"
    -DDIR_PLUGINS="${PLUGINS_DIRECTORY}"
)

if (APPLE)
    # Static version on MacOSX
    set(wxWidgets_USE_STATIC On)
endif ()

# wxWidgets
if (WIN32)
    # On Windows the scintilla library is required
    find_package(wxWidgets REQUIRED xrc xml adv gl stc scintilla core base)
else ()
    find_package(wxWidgets REQUIRED xrc xml adv gl stc core base)
endif ()

include(${wxWidgets_USE_FILE})

# ######################################################################### #

# Sources
set(SRCS
    main.cpp
    MainFrame.h
    MainFrame.cpp
    AboutDialog.h
    AboutDialog.cpp
    PluginsDialog.h
    PluginsDialog.cpp
    wxcrafter.h
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    CanvasWidget.hpp
    CanvasWidget.cpp
    SimulatorThread.hpp
    SimulatorThread.cpp
    Logger.hpp
    Logger.cpp
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

set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}-gui")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015 Georgiev Lab")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "cz.zcu.ccy.${APP_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "${APP_NAME}.icns")

# ######################################################################### #

# Create program
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SRCS}
)

# ######################################################################### #

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist.in
    )
endif ()

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    loader-xml
    simulator
    ${wxWidgets_LIBRARIES}
)

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (UNIX AND NOT APPLE)
    # Configure desktop entry
    configure_file(app.desktop ${PROJECT_NAME}.desktop)

    # Install desktop entry
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop
        DESTINATION /usr/share/applications
    )
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
    BUNDLE DESTINATION ${INSTALL_DIR_BUNDLE}
)

# Install icons
if (UNIX AND NOT APPLE)
    install(FILES ${CMAKE_SOURCE_DIR}/icons/icon_128x128.png
        DESTINATION ${INSTALL_DIR_DATA}/icons
    )
endif ()

# ######################################################################### #

# Fixup shared libraries for bundle
if (APPLE)
    set(APP "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_BUNDLE}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app")
    set(DIRS "/usr/local/lib;\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY}")

    install(CODE
        "
        file(GLOB PLUGINS RELATIVE \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}\"
            \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
        )
        include(BundleUtilities)
        fixup_bundle(\"${APP}\" \"${PLUGINS}\" \"${DIRS}\")
        "
        COMPONENT Runtime
    )
endif ()

# ######################################################################### #
