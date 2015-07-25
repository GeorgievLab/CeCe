# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Create project
project(simulator CXX)

# Include Python API
if (UNIX AND NOT APPLE)
    # Link boost dynamically on Linux
    set(Boost_USE_STATIC_LIBS Off)
endif ()

find_package(Boost REQUIRED filesystem system)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# Sources
set(SRCS
    Object.hpp
    Object.cpp
    Simulation.hpp
    Simulation.cpp
    SimulationFactory.hpp
    SimulationFactory.cpp
    Simulator.hpp
    Simulator.cpp
    Module.hpp
    Plugin.hpp
    Plugin.cpp
    PluginApi.hpp
    PluginManager.hpp
    PluginManager.cpp
    Configuration.hpp
    Configuration.cpp
    Shape.hpp
    ShapeToGrid.hpp
    Program.hpp
    Obstacle.hpp
    Obstacle.cpp
    SimulationListener.hpp

    # Add object libraries
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:parser>
)

if (ENABLE_RENDER)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

    set(RENDER_LIBS ${OPENGL_LIBRARIES})
    set(SRCS ${SRCS} $<TARGET_OBJECTS:render>)
endif ()

# Include build-in libraries
set(PLUGINS_LIBS "")
set(BUILTIN_PLUGINS "")

# Foreach build-in plugins
foreach (PLUGIN ${PLUGINS_BUILTIN})
    message(STATUS "Builtin plugin: ${PLUGIN}")
    plugin_project_name(${PLUGIN} PLUGIN_PROJECT)
    set(PLUGINS_LIBS ${PLUGINS_LIBS} ${PLUGIN_PROJECT})
    # Fix module name: replace '-' by '_'
    string(REPLACE "-" "_" FIXED_PLUGIN ${PLUGIN})
    set(BUILTIN_PLUGINS "${BUILTIN_PLUGINS}ITEM(${PLUGIN},${FIXED_PLUGIN})")
endforeach ()

# Pass build-in libraries list
add_definitions(-DBUILTIN_PLUGINS=${BUILTIN_PLUGINS})

# Set path to plugins directory
add_definitions(-DDIR_PLUGINS="${PLUGINS_DIRECTORY}")

if (WIN32)
    add_definitions(-DSIMULATOR_DLL_EXPORT=1)
endif ()

message(STATUS "Build simulator library as shared")

# Create library
add_library(${PROJECT_NAME} SHARED
    ${SRCS}
)

# Set framework flag
set_target_properties(${PROJECT_NAME} PROPERTIES FRAMEWORK 1)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${PLUGINS_LIBS}
    ${RENDER_LIBS}
    ${Boost_LIBRARIES}
)

if (ENABLE_BUILTIN_PHYSICS)
    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        ${BOX2D_LIBRARIES}
    )
endif ()

if (MINGW)
    target_link_libraries(${PROJECT_NAME}
        Ws2_32
    )
endif ()

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}"
)

# Install
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${INSTALL_DIR_LIBRARY}
    FRAMEWORK ${INSTALL_DIR_FRAMEWORK}
)

# ######################################################################### #
