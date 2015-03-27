# ######################################################################### #

project(render)

add_definitions(-DRENDER_USE_VBO=1)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
)

# Create library
add_library(render
    Position.hpp
    Color.hpp
    Context.hpp
    Context.cpp
    Camera.hpp
    Camera.cpp
    Drawable.hpp
    Circle.hpp
    Circle.cpp
)

# Setup dependencies
add_dependencies(render
    core
    simulator
)

target_link_libraries(render
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# ######################################################################### #
