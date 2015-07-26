# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# CPACK
include(InstallRequiredSystemLibraries)

# ######################################################################### #

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

# ######################################################################### #

set(CPACK_STRIP_FILES "${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}-cli;${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}-gui")
set(CPACK_PACKAGE_EXECUTABLES
    "${CMAKE_PROJECT_NAME}-cli" "${APP_NAME}"
    "${CMAKE_PROJECT_NAME}-cli" "${APP_NAME} GUI"
)

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
else (UNIX AND NOT APPLE)
    set(CPACK_SOURCE_STRIP_FILES "")
    # autogenerate dependency information
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif ()

include(CPack)

# ######################################################################### #
