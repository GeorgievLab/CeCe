# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Project
project(cli CXX)

# ######################################################################### #

# Options
option(CONFIG_CLI_ENABLE_VIDEO_CAPTURE "Enable support for video capturing" On)

# ######################################################################### #

if (ENABLE_RENDER)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIR})
endif ()

# Set path to plugins directory
add_definitions(-DDIR_PLUGINS="${PLUGINS_DIRECTORY}")

if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
    if (NOT ENABLE_RENDER)
        message(FATAL_ERROR "Video capture requires rendering support")
    endif ()

    # Add support for OpenCV video capturing
    find_package(OpenCV)

    # OpenCV not found
    if (OpenCV_FOUND)
        message(STATUS "Video capture support: OpenCV ${OpenCV_VERSION}")
        include_directories(${OpenCV_INCLUDE_DIR})
        add_definitions(-DCONFIG_CLI_ENABLE_VIDEO_CAPTURE)
    else ()
        message(WARNING "OpenCV not found, disabling video capture")
    endif ()
endif ()

# ######################################################################### #

# Sources
set(SRCS
    main.cpp
)

if (APPLE)
    set(SRCS ${SRCS} icons/${APP_NAME}.icns)
    set_source_files_properties(icons/${APP_NAME}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

# ######################################################################### #

set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}-cli")
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

if (ENABLE_RENDER)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARIES}
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE AND OpenCV_FOUND)
    target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
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
        run.sh
        DESTINATION "${INSTALL_DIR_RUNTIME}"
        COMPONENT Runtime
    )

    set(APP "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_BUNDLE}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app")
    set(DIRS "/usr/local/lib;\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY}")

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

# ######################################################################### #
