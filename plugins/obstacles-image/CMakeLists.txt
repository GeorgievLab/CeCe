# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Find OpenCV
if (UNIX AND NOT APPLE)
    # OpenCV 2 (Ubuntu LTS)
    find_package(OpenCV REQUIRED core imgproc highgui)
else ()
    # OpenCV 3
    find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
endif ()

# Include OpenCV
include_directories(${OpenCV_INCLUDE_DIR})

# ######################################################################### #

# Sources
set(SRCS
    Library.cpp
)

# Build plugin
build_plugin(obstacles-image
    SOURCES ${SRCS}
    LIBRARIES ${OpenCV_LIBRARIES}
)

# ######################################################################### #
