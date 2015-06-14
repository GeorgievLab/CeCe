# ######################################################################### #

# Project name
set(PROJECT_NAME gui)

project(${PROJECT_NAME} CXX)

# wxWidgets
if (WIN32)
    # On Windows the scintilla library is required
    find_package(wxWidgets REQUIRED core base scintilla stc gl xml xrc adv)
else ()
    find_package(wxWidgets REQUIRED core base stc gl xml xrc adv)
endif ()

include(${wxWidgets_USE_FILE})

# Sources
set(SRCS
    main.cpp
    MainFrame.h
    MainFrame.cpp
    AboutDialog.h
    AboutDialog.cpp
    ModulesDialog.h
    ModulesDialog.cpp
    wxcrafter.h
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    CanvasWidget.hpp
    CanvasWidget.cpp
    SimulatorThread.hpp
    SimulatorThread.cpp
    Logger.hpp
    Logger.cpp
)

# Create program
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${SRCS}
    )
    else ()
    add_executable(${PROJECT_NAME}
        ${SRCS}
    )
endif ()

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "cell-sim-gui"
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
target_link_libraries(${PROJECT_NAME}
    ${MODULES_LIBS}
    simulator
    parser-xml
    ${wxWidgets_LIBRARIES}
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .   COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

# ######################################################################### #
