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

# ######################################################################### #

# Create executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    main.cpp
)

# ######################################################################### #

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    loader-xml
    loader-reactions
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
)

# Fixup shared libraries for bundle
if (APPLE)
#    install(CODE "
#        execute_process(COMMAND
#            ${CMAKE_SOURCE_DIR}/platform/MacOS/bundle.sh
#            \${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}
#        )
#    " COMPONENT Runtime)
#    set(APP "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_BUNDLE}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app")
#
#    install(CODE
#        "
#        include(BundleUtilities)
#        fixup_bundle(\"${APP}\" \"\" \"/usr/local\")
#        "
#        COMPONENT Runtime
#    )
endif ()

# ######################################################################### #
