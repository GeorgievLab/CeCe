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
    find_package(wxWidgets REQUIRED xrc xml adv gl stc scintilla core base)
else ()
    find_package(wxWidgets REQUIRED xrc xml adv gl stc core base)
endif ()

include(${wxWidgets_USE_FILE})

# Sources
set(SRCS
    main.cpp
    MainFrame.h
    MainFrame.cpp
    AboutDialog.h
    AboutDialog.cpp
    PluginsDialog.h
    PluginsDialog.cpp
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
    loader-xml
    simulator
    ${wxWidgets_LIBRARIES}
)

if (UNIX AND NOT APPLE)
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
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
    BUNDLE DESTINATION ${INSTALL_DIR_BUNDLE}
)

# Install icons
if (NOT APPLE)
    install(FILES ${CMAKE_SOURCE_DIR}/icons/icon_128x128.png
        DESTINATION ${INSTALL_DIR_DATA}/icons
    )
endif ()

# Fixup shared libraries for bundle
if (APPLE)
#    install(CODE "
#        execute_process(COMMAND
#            ${CMAKE_SOURCE_DIR}/platform/MacOS/bundle.sh
#            \${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}
#        )
#    " COMPONENT Runtime)
    set(APP "\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_BUNDLE}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}.app")
    
    install(CODE
        "
        include(BundleUtilities)
        fixup_bundle(\"${APP}\" \"\" \"\")
        "
        COMPONENT Runtime
    )
endif ()

# ######################################################################### #
