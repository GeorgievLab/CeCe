# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# CPACK
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cell simulator")
set(CPACK_PACKAGE_VENDOR "Georgiev Lab")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${APP_NAME} ${VERSION_MAJOR}.${VERSION_MINOR}")
set(CPACK_PACKAGE_NAME ${APP_NAME})
set(CPACK_PACKAGE_CONTACT "Jiří Fatka <fatkaj@ntis.zcu.cz>")

if (WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${APP_NAME}")
    #set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
    #set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
    #set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif (APPLE)
    # Configure Info.plist
    if (ENABLE_GUI)
        set(APPLE_GUI_EXECUTABLE ${CMAKE_PROJECT_NAME}-gui)
    elseif (ENABLE_CLI)
        set(APPLE_GUI_EXECUTABLE ${CMAKE_PROJECT_NAME}-cli)
    endif ()
    set(APPLE_GUI_ICON "${CPACK_PACKAGE_NAME}.icns")
    set(APPLE_GUI_IDENTIFIER "cz.zcu.ccy.simulator")
    set(APPLE_GUI_BUNDLE_NAME ${CPACK_PACKAGE_NAME})
    set(APPLE_GUI_SHORT_VERSION_STRING ${CPACK_PACKAGE_VERSION})
    set(APPLE_GUI_BUNDLE_VERSION ${CPACK_PACKAGE_VERSION})
    set(APPLE_GUI_COPYRIGHT "${CPACK_PACKAGE_VENDOR} (c) 2015")
    configure_file(${CMAKE_SOURCE_DIR}/platform/MacOS/Info.plist "${PROJECT_BINARY_DIR}/Info.plist")

    # Setup bundle
    set(CPACK_BUNDLE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/icons/icon.icns")
    set(CPACK_BUNDLE_PLIST "${PROJECT_BINARY_DIR}/Info.plist")
else ()
    set(CPACK_STRIP_FILES "bin/cell-sim-gui")
    set(CPACK_SOURCE_STRIP_FILES "")
    # autogenerate dependency information
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif ()

set(CPACK_PACKAGE_EXECUTABLES "${APP_NAME}" "${APP_NAME}")

include(CPack)

# ######################################################################### #
