# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (!ENABLE_RENDER)
    message(FATAL_ERROR "Cannot build module without ENABLE_RENDER")
endif ()

# Use Magick++
find_package(ImageMagick COMPONENTS Magick++)

# Include diretories
include_directories(${ImageMagick_INCLUDE_DIRS})

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
)

# Build plugin
build_plugin(picture
    SOURCES ${SRCS}
    LIBRARIES ${ImageMagick_LIBRARIES}
)

# ######################################################################### #
