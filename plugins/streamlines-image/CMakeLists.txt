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
include_directories(${OpenCV_INCLUDE_DIRS})

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

if (WIN32)
    set(LIBRARIES "")

    foreach (DIR ${OpenCV_LIB_DIR})
        set(DIR "${DIR}/../bin")

        foreach (COMPONENT ${OpenCV_LIB_COMPONENTS})
            file(GLOB FILES "${DIR}/lib${COMPONENT}*.dll")
            set(LIBRARIES ${LIBRARIES} ${FILES})
        endforeach ()
    endforeach ()

    list(REMOVE_DUPLICATES LIBRARIES)

    install(FILES ${LIBRARIES}
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

# ######################################################################### #
