# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Project name
set(PROJECT_NAME simulator)

# Create project
project(${PROJECT_NAME} CXX)

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
    Library.hpp
    Library.cpp
    LibraryApi.hpp
    Configuration.hpp
    Configuration.cpp
    Shape.hpp
    ShapeToGrid.hpp
    Program.hpp

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
set(BUILDIN_LIBRARIES "")

# Foreach build-in plugins
foreach (PLUGIN ${PLUGINS_BUILDIN})
    message("-- Build-in plugin: ${PLUGIN}")
    set(PLUGINS_LIBS ${PLUGINS_LIBS} module-${PLUGIN})
    # Fix module name: replace '-' by '_'
    string(REPLACE "-" "_" FIXED_PLUGIN ${PLUGIN})
    set(BUILDIN_LIBRARIES "${BUILDIN_LIBRARIES}ITEM(${PLUGIN},${FIXED_PLUGIN})")
endforeach ()

# Pass build-in libraries list
add_definitions(-DBUILDIN_LIBRARIES=${BUILDIN_LIBRARIES})

if (WIN32)
    add_definitions(-DSIMULATOR_DLL_EXPORT=1)
endif ()

message("-- Build simulator library as shared")

# Create library
add_library(${PROJECT_NAME} SHARED
    ${SRCS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${PLUGINS_LIBS}
    ${RENDER_LIBS}
)

if (ENABLE_BUILDIN_PHYSICS)
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
    OUTPUT_NAME "cell-sim-core"
)

# Install
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${INSTALL_DIR_LIBRARY}
    FRAMEWORK ${INSTALL_DIR_FRAMEWORK}
)

# ######################################################################### #
