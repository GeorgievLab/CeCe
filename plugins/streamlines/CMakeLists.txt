# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.cpp
    Lattice.hpp
    Lattice.cpp
    LatticeData.hpp
    LatticeData.cpp
    ObstacleMap.hpp
    StoreState.hpp
    StoreState.cpp
    StoreObjectState.hpp
    StoreObjectState.cpp
)

# Options
option(DEV_streamlines_RENDER "Enable or disable streamlines rendering" Off)

# Enable debug rendering
if (DEV_streamlines_RENDER)
    add_definitions(-DDEV_DRAW_VELOCITY=1)
endif ()

# Build plugin
build_plugin(streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
