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

if (ENABLE_RENDER)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIR})
endif ()

# Set path to plugins directory
add_definitions(-DDIR_PLUGINS="${PLUGINS_DIRECTORY}")

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
