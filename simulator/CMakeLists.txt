# ######################################################################### #

# Project name
set(PROJECT_NAME simulator)

# Create project
project(${PROJECT_NAME} CXX)

option(ENABLE_SIMULATOR_STATIC "Enable or disable static build of the simulator library. Static build doen't support dynamic modules." Off)

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
set(MODULES_LIBS "")
set(BUILDIN_LIBRARIES "")
set(DYNAMIC_MODULES ${MODULES})

foreach (MODULE ${MODULES_BUILDIN})
    message("-- Build-in module: ${MODULE}")
    set(MODULES_LIBS ${MODULES_LIBS} module-${MODULE})
    set(BUILDIN_LIBRARIES "${BUILDIN_LIBRARIES}ITEM(${MODULE})")
    list(REMOVE_ITEM DYNAMIC_MODULES ${MODULE})
endforeach ()

# Pass build-in libraries list
add_definitions(-DBUILDIN_LIBRARIES=${BUILDIN_LIBRARIES})

# If there is dynamic module, force shared library
list(LENGTH DYNAMIC_MODULES DYNAMIC_MODULES_LEN)
if (DYNAMIC_MODULES_LEN GREATER 0)
    set(ENABLE_SIMULATOR_STATIC Off)
endif ()

if (ENABLE_SIMULATOR_STATIC)
    message("-- Build simulator library as static")

    add_definitions(-DSTATIC_BUILD=1)

    # Create library
    add_library(${PROJECT_NAME}
        ${SRCS}
    )
else ()
    message("-- Build simulator library as shared")

    add_definitions(-DSIMULATOR_DLL_EXPORT=1)

    # Create library
    add_library(${PROJECT_NAME} SHARED
        ${SRCS}
    )
endif ()

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${MODULES_LIBS}
)

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "cell-sim-core"
)

if (ENABLE_BUILDIN_PHYSICS)
    # Add dependency
    add_dependencies(${PROJECT_NAME}
        Box2D
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        Box2D
    )
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# ######################################################################### #
