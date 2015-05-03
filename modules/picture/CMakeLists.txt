# ######################################################################### #

if (!ENABLE_RENDER)
    message(FATAL_ERROR "Cannot build module without ENABLE_RENDER")
endif ()

# Module options
option(ENABLE_PNG "Enable or disable PNG support" On)
option(ENABLE_JPEG "Enable or disable JPEG support" Off) # Not supported yet

# Project name
set(PROJECT_NAME module-picture)

# Create project
project(${PROJECT_NAME} CXX)

# Additional libraries
set(LIBRARIES "")

# PNG support
if (ENABLE_PNG)
    message("-- PNG support enabled")

    find_package(PNG REQUIRED)

    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(-DPNG_SUPPORT=1 ${PNG_DEFINITIONS})
    set(LIBRARIES ${LIBRARIES} ${PNG_LIBRARIES})
endif ()

# JPEG support
if (ENABLE_JPEG)
    message("-- JPEG support enabled")

    find_package(JPEG REQUIRED)

    include_directories(${JPEG_INCLUDE_DIR})
    add_definitions(-DJPEG_SUPPORT=1)
    set(LIBRARIES ${LIBRARIES} ${JPEG_LIBRARIES})
endif ()

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
    formats.hpp
    formats.cpp
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
    ${LIBRARIES}
)

# ######################################################################### #
