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
    Library.cpp
    wrapper_module.hpp
    wrapper_module.cpp
)

# Build plugin
build_plugin(streamlines-python
    SOURCES ${SRCS}
    PLUGINS_REQUIRED streamlines python
    LIBRARIES ${PYTHON_LIBRARIES}
)

# ######################################################################### #
