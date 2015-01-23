# ######################################################################### #

project(gui)

# wxWidgets
find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
include(${wxWidgets_USE_FILE})

# Create program
add_executable(gui
    main.cpp
    MainFrame.h
    MainFrame.cpp
    wxcrafter.h
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    CanvasWidget.h
    CanvasWidget.cpp
    SimulatorThread.hpp
    SimulatorThread.cpp
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
