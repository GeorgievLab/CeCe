# ######################################################################### #

project(gui)

# Options
option(ENABLE_GUI_SCREENSHOT    "Enable or disable screenshot support in GUI" On)

# wxWidgets
find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
include(${wxWidgets_USE_FILE})

if (ENABLE_GUI_SCREENSHOT)
    # Image support
    find_package(ImageMagick COMPONENTS Magick++)

    if (ImageMagick_FOUND)
        message("-- Screenshot support enabled")

        # Include directories
        include_directories(${ImageMagick_INCLUDE_DIRS})

        # Add definitions
        add_definitions(-DENABLE_SCREENSHOT=1)
    endif ()
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
set(MODULES_SHARED ${MODULES})

# Remove buildin
foreach (MODULE ${MODULES_BUILDIN})
    list(REMOVE_ITEM MODULES_SHARED ${MODULE})
endforeach ()

foreach (MODULE ${MODULES_SHARED})
    message("-- Shared module: ${MODULE}")
    set(MODULES_LIBS ${MODULES_LIBS} module-${MODULE})
endforeach ()

# Link libraries
target_link_libraries(gui
    ${MODULES_LIBS}
    simulator
    parser-xml
    ${wxWidgets_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    -pthread
)

# Setup dependencies
add_dependencies(gui
    simulator
    parser-xml
)

# ######################################################################### #
