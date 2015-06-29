# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Include Python API
find_package(PythonLibs REQUIRED)

# Include directories
include_directories(${PYTHON_INCLUDE_DIRS})

# Sources
set(SRCS
    Handle.hpp
    View.hpp
    Exception.hpp
    Exception.cpp
    ValueCast.hpp
    Source.hpp
    Source.cpp
    Module.hpp
    Module.cpp
    Program.hpp
    Program.cpp
    Object.hpp
    Object.cpp
    Library.cpp
    wrapper.hpp
    wrapper.cpp
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

# Build plugin
build_plugin(python
    SOURCES ${SRCS}
    LIBRARIES ${PYTHON_LIBRARIES}
)

# ######################################################################### #
