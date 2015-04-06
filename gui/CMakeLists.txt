# ######################################################################### #

project(gui)

if (ENABLE_RENDER)
    set(RENDER_LIBS GL GLU)
    set(RENDER_DEP render)
endif ()

# wxWidgets
find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
include(${wxWidgets_USE_FILE})

# PNG support
find_package(PNG)

if (PNG_FOUND)
    message("-- PNG support enabled")
    add_definitions(-DPNG_SUPPORT ${PNG_DEFINITIONS})
endif ()

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
    PngImage.h
    PngImage.cpp
)

# Link libraries
target_link_libraries(gui
    parser
    simulator
    core
    ${RENDER_DEP}
    ${RENDER_LIBS}
    ${wxWidgets_LIBRARIES}
    ${PNG_LIBRARIES}
)

# Setup dependencies
add_dependencies(gui
    simulator
    parser
    core
    ${RENDER_DEP}
)

# ######################################################################### #
