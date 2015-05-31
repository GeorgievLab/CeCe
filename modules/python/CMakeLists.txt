# ######################################################################### #

# Project name
set(PROJECT_NAME module-python)

# Create project
project(${PROJECT_NAME} CXX)

# Include Python API
find_package(PythonLibs REQUIRED)

# Include directories
include_directories(
    ${PYTHON_INCLUDE_DIRS}
)

# Sources
set(SRCS
    Handle.hpp
    Exception.hpp
    Exception.cpp
    Source.hpp
    Source.cpp
    Module.hpp
    Module.cpp
    Program.hpp
    Program.cpp
    Object.hpp
    Object.cpp
    Library.hpp
    Library.cpp
    wrapper_stdout.hpp
    wrapper_stdout.cpp
    wrapper_core.hpp
    wrapper_core.cpp
    wrapper_render.hpp
    wrapper_render.cpp
    wrapper_parser.hpp
    wrapper_parser.cpp
    wrapper_simulator.hpp
    wrapper_simulator.cpp
)

# Build static / dynamic
list(FIND MODULES_BUILDIN "cell" IS_STATIC)

if (${IS_STATIC} GREATER -1)
    message("-- Module ${PROJECT_NAME} is build-in")

    if (UNIX)
        add_compile_options(-fPIC)
    endif ()

    add_definitions(-DLIBRARY_BUILDIN=1)
    add_library(${PROJECT_NAME} ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        ${PYTHON_LIBRARIES}
    )
else ()
    message("-- Module ${PROJECT_NAME} is dynamic")
    add_library(${PROJECT_NAME} SHARED ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
        simulator
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        simulator
        ${PYTHON_LIBRARIES}
    )
endif ()

# ######################################################################### #
