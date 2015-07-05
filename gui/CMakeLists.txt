# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Project name
project(gui CXX)

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
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SRCS}
)

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

set(PLUGINS_TO_LINK "")

if (ENABLE_PLUGINS_DIRECT_LINK)
    plugin_project_names("${PLUGINS_BUILD}" PLUGINS_TO_LINK)
endif ()

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    parser-xml
    ${PLUGINS_TO_LINK}
    ${wxWidgets_LIBRARIES}
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${INSTALL_DIR_BUNDLE}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
)

# Fixup bundle on MacOS X
if (APPLE)
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/cell-sim-gui.app\" \"\" \"/usr/local/lib\")
    " COMPONENT Runtime)
endif ()

# ######################################################################### #
