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
    wrapper_CellBase.hpp
    wrapper_CellBase.cpp
    wrapper_Yeast.hpp
    wrapper_Yeast.cpp
)

# Build plugin
build_plugin(cell-python
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell python
    LIBRARIES ${PYTHON_LIBRARIES}
)

# ######################################################################### #
