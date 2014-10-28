# ######################################################################### #

project(gui)

# wxWidgets
find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
include(${wxWidgets_USE_FILE})

# Sources
set(CXX_SRCS
    main.cpp
    MainFrame.cpp
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    CanvasWidget.cpp
)

# Create program
add_executable(gui
    ${CXX_SRCS}
)

# Link libraries
target_link_libraries(gui
    javascript
    simulator
    physics
    render
    v8
    BulletDynamics
    BulletCollision
    LinearMath
    GL
    GLU
    ${wxWidgets_LIBRARIES}
)

# Setup dependencies
add_dependencies(gui javascript simulator core render physics)

# ######################################################################### #
