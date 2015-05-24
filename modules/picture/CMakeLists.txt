# ######################################################################### #

if (!ENABLE_RENDER)
    message(FATAL_ERROR "Cannot build module without ENABLE_RENDER")
endif ()

# Project name
set(PROJECT_NAME module-picture)

# Create project
project(${PROJECT_NAME} CXX)

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

# Create library
if (MODULE_BUILD_STATIC)
    add_library(${PROJECT_NAME} ${SRCS})
else ()
    add_library(${PROJECT_NAME} SHARED ${SRCS})
endif ()

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    core
    simulator
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    ${ImageMagick_LIBRARIES}
)

# ######################################################################### #
