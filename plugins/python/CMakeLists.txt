# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (WIN32)
    set(PYTHON_LIB_PATH "C:/Windows/SysWOW64/python27.dll" CACHE STRING "Path to python2 library")
endif ()

# ######################################################################### #

# Include Python API
find_package(PythonLibs REQUIRED)

# Include directories
include_directories(${PYTHON_INCLUDE_DIRS})

# ######################################################################### #

# Sources
set(SRCS
    Handle.hpp
    Handle.cpp
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
    Initializer.hpp
    Initializer.cpp
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

if (WIN32 AND PYTHON_LIB_PATH)
    # Install examples
    install(FILES ${PYTHON_LIB_PATH}
        DESTINATION "${INSTALL_DIR_LIBRARY}"
    )
endif ()

# ######################################################################### #
