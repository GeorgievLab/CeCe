# ######################################################################### #

project(gui)

# wxWidgets
find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
include(${wxWidgets_USE_FILE})

# Image support
find_package(ImageMagick COMPONENTS Magick++)

if (ImageMagick_FOUND)
    message("-- Image save support")

    # Include directories
    include_directories(${ImageMagick_INCLUDE_DIRS})

    # Add definitions
    add_definitions(-DENABLE_IMAGES=1)
endif ()

# Create program
add_executable(gui
    main.cpp
    MainFrame.h
    MainFrame.cpp
    wxcrafter.h
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    CanvasWidget.hpp
    CanvasWidget.cpp
    SimulatorThread.hpp
    SimulatorThread.cpp
)

set(MODULES_LIBS "")

foreach (MODULE ${MODULES})
    set(MODULES_LIBS ${MODULES_LIBS} module-${MODULE})
endforeach ()

# Link libraries
target_link_libraries(gui
    ${MODULES_LIBS}
    simulator
    parser-xml
    ${wxWidgets_LIBRARIES}
    ${ImageMagick_LIBRARIES}
)

# Setup dependencies
add_dependencies(gui
    simulator
    parser-xml
)

# ######################################################################### #
