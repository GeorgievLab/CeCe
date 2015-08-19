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

# Use PNG images
find_package(PNG REQUIRED)

# Include diretories
include_directories(${PNG_INCLUDE_DIRS})

# PNG definitions
add_definitions(${PNG_DEFINITIONS})

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.cpp
)

# Build plugin
build_plugin(picture
    SOURCES ${SRCS}
    LIBRARIES ${PNG_LIBRARIES}
)

# ######################################################################### #
