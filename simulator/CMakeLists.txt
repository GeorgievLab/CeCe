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
    Configuration.hpp
    Shape.hpp

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

# Create library
add_library(${PROJECT_NAME} SHARED
    ${SRCS}
)

# Use different name
#set_target_properties(${PROJECT_NAME} PROPERTIES
#    OUTPUT_NAME "sim"
#)

if (USE_BUILDIN_PHYSICS)
    # Add dependency
    add_dependencies(${PROJECT_NAME}
        Box2D
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        Box2D
    )
endif ()

# ######################################################################### #
