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

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    parser-xml
    ${wxWidgets_LIBRARIES}
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (UNIX AND NOT APPLE)
    # Configure desktop entry
    configure_file(app.desktop ${PROJECT_NAME}.desktop)

    # Install desktop entry
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop
        DESTINATION /usr/share/applications
    )
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${INSTALL_DIR_BUNDLE}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
)

# Install icons
install(FILES ${CMAKE_SOURCE_DIR}/icons/icon_128x128.png
    DESTINATION ${INSTALL_DIR_DATA}/icons
)

# Fixup bundle on MacOS X
if (APPLE)
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app\" \"\" \"/usr/local/lib\")
    " COMPONENT Runtime)
endif ()

# ######################################################################### #
