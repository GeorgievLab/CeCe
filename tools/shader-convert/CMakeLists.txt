# ######################################################################### #

# Project name
set(PROJECT_NAME tool-shader-convert)

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    main.cpp
)

# Create program
add_executable(${PROJECT_NAME} ${SRCS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
)

# ######################################################################### #
