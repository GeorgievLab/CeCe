# ######################################################################### #

project(render)

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
    errors.hpp
    errors.cpp
    Context.hpp
    Context.cpp
    Camera.hpp
    Camera.cpp
    Drawable.hpp
    Drawable.cpp
    Circle.hpp
    Circle.cpp
    Grid.hpp
    Grid.cpp
    GridVector.hpp
    GridVector.cpp
    GridValue.hpp
    GridValue.cpp
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
