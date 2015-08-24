# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Options
if (CONFIG_RENDER_ENABLE)
    option(DEV_PLUGIN_streamlines_RENDER "Enable or disable streamlines rendering" Off)
    option(DEV_PLUGIN_streamlines_VELOCITY_RENDER "Enable or disable streamlines velocity rendering" Off)
    option(DEV_PLUGIN_streamlines_FORCE_RENDER "Enable or disable streamlines force rendering" Off)
endif ()

# ######################################################################### #

# Enable debug rendering
if (DEV_PLUGIN_streamlines_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_RENDER=1)
endif ()

if (DEV_PLUGIN_streamlines_VELOCITY_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_VELOCITY_RENDER=1)
endif ()

if (DEV_PLUGIN_streamlines_FORCE_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_FORCE_RENDER=1)
endif ()

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

# Build plugin
build_plugin(streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
