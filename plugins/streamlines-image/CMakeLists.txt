# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV
include_directories(${OpenCV_INCLUDE_DIR})

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
)

# Build plugin
build_plugin(streamlines-image
    SOURCES ${SRCS}
    LIBRARIES ${OpenCV_LIBRARIES}
    PLUGINS_REQUIRED
        streamlines
)

# ######################################################################### #
