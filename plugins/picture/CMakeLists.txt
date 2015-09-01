# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (!CONFIG_RENDER_ENABLE)
    message(FATAL_ERROR "Cannot build module without CONFIG_RENDER_ENABLE")
endif ()

# Find OpenCV
if (UNIX AND NOT APPLE)
    # OpenCV 2 (Ubuntu LTS)
    find_package(OpenCV REQUIRED core imgproc highgui)
else ()
    # OpenCV 3
    find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
endif ()

# Include OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# ######################################################################### #

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.cpp
)

# Build plugin
build_plugin(picture
    SOURCES ${SRCS}
    LIBRARIES ${OpenCV_LIBRARIES}
)

# ######################################################################### #
