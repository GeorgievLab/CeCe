# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Find OpenCV
find_package(OpenCV REQUIRED)

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.cpp
)

# Options
option(DEV_streamlines_image_CONTOURS "Enable or disable contours rendering" Off)

# Enable debug rendering
if (DEV_streamlines_RENDER)
    add_definitions(-DDEV_DRAW_VELOCITY=1)
endif ()

# Draw contours
if (DEV_streamlines_image_CONTOURS)
    add_definitions(-DDEV_DRAW_CONTOURS=1)
endif ()

# Build plugin
build_plugin(streamlines-image
    SOURCES ${SRCS}
    LIBRARIES ${OpenCV_LIBS}
    PLUGINS_REQUIRED
        streamlines
)

# ######################################################################### #
