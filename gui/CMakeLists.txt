# ######################################################################### #

project(gui)

if (ENABLE_RENDER)
    set(RENDER_LIBS GL GLU)
    set(RENDER_DEP render)
endif ()

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
    VectorCtrlBase.h
    VectorIntCtrl.h
    VectorIntCtrl.cpp
    VectorFloatCtrl.h
    VectorFloatCtrl.cpp
)

# Link libraries
target_link_libraries(gui
    parser
    simulator
    core
    ${RENDER_DEP}
    ${RENDER_LIBS}
    ${wxWidgets_LIBRARIES}
)

# Setup dependencies
add_dependencies(gui
    simulator
    parser
    core
    ${RENDER_DEP}
)

# ######################################################################### #
