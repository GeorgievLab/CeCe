# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(cell-sim)

# Add path to additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# Applicatio version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 1)
set(VERSION_SPECIAL "alpha")

# Setup macros with application version
add_definitions(
    -DVERSION_MAJOR=${VERSION_MAJOR}
    -DVERSION_MINOR=${VERSION_MINOR}
    -DVERSION_PATCH=${VERSION_PATCH}
    -DVERSION_SPECIAL="${VERSION_SPECIAL}"
)

# Options
option(ENABLE_GUI                   "Enable GUI" On)
option(ENABLE_CLI                   "Enable CLI" Off)
option(ENABLE_RENDER                "Enable simulation rendering" On)
option(ENABLE_SSE                   "Enable SSE instructions" On)
option(ENABLE_BUILDIN_PHYSICS       "Enable build-in physics engine (Box2D)" On)
option(ENABLE_MEASUREMENT           "Enable or disable time measurement" Off)
option(ENABLE_BUILDIN_PHYSICS_DEBUG "Enable or disable physics engine debugging" Off)

option(BUILD_TOOLS                  "Build support tools" Off)
set(MODULES                         "cylinder-streamlines;diffusion;diffusion-cylinder-streamlines;cell;python" CACHE STRING "A list of build modules")
set(MODULES_BUILDIN                 "" CACHE STRING "A list of build-in modules")

# Options specific for different compilers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    option(CLANG_ASAN               "If address sanitizer should be used" Off)
    option(CLANG_MSAN               "If memory sanitizer should be used" Off)
    option(CLANG_USE_LIBCPP         "If libc++ should be used" Off)

    # C++11 required
    add_compile_options(-pedantic-errors -pedantic -std=c++11)

    # Use libc++
    if (CLANG_USE_LIBCPP)
        add_compile_options(-stdlib=libc++)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    endif()

    # Debug flags
    if (CLANG_ASAN)
        message("-- Using address sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    elseif (CLANG_MSAN)
        message("-- Using memory sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC

    # C++11 required
    add_compile_options(-pedantic-errors -pedantic -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
endif()

# Default include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# GUI enabled
if (ENABLE_GUI)
    message("-- Build GUI application")
    set(ENABLE_RENDER 1)
else ()
    # If GUI is disabled build CLI
    set(ENABLE_CLI 1)
endif ()

# CLI enabled
if (ENABLE_CLI)
    message("-- Build CLI application")
endif ()

# Rendering enabled
if (ENABLE_RENDER)
    message("-- OpenGL rendering support")
    add_definitions(-DENABLE_RENDER=1)
endif ()

if (ENABLE_SSE)
    # TODO: detect SSE support
    message("-- Use SSE instructions")
    add_definitions(-DENABLE_SSE=1)
endif ()

# Build-in physics enabled
if (ENABLE_BUILDIN_PHYSICS)
    message("-- Using build-in physics (Box2D)")
    find_package(Box2D REQUIRED)
    include_directories(${Box2D_INCLUDE_DIRS})

    add_definitions(-DENABLE_PHYSICS=1)

    if (ENABLE_BUILDIN_PHYSICS_DEBUG)
        message("-- Physics engine debugging enabled")
        add_definitions(-DENABLE_PHYSICS_DEBUG=1)
    endif ()
endif ()

if (ENABLE_MEASUREMENT)
    message("-- Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

set(DYNAMIC_MODULES ${MODULES})

# Remove missing modules
foreach (MODULE ${MODULES})
    if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${MODULE}")
        message("-- Module '${MODULE}' found, build")
    else ()
        message("-- Module '${MODULE}' not found, ignore")
        list(REMOVE_ITEM MODULES ${MODULE})
        list(REMOVE_ITEM MODULES_BUILDIN ${MODULE})
    endif ()
    list(REMOVE_ITEM DYNAMIC_MODULES ${MODULE})
endforeach ()

# Default libraries
add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(parser-xml)

# Add render library
if (ENABLE_RENDER)
    add_subdirectory(render)
endif ()

if (ENABLE_BUILDIN_PHYSICS)
    # Link libraries
    link_directories(${Box2D_LIBRARIES})
endif ()

# Main simulator project
add_subdirectory(simulator)

# Modules
foreach (MODULE ${MODULES})
    add_subdirectory(modules/${MODULE})
endforeach ()

# Build GUI project
if (ENABLE_GUI)
    add_subdirectory(gui)
endif ()

# Build CLI application
if (ENABLE_CLI)
    add_subdirectory(cli)
endif ()

if (BUILD_TOOLS)
    add_subdirectory(tools/shader-convert)
endif()

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
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Simulator ${VERSION_MAJOR}.${VERSION_MINOR}")
set(CPACK_PACKAGE_NAME "Simulator")
set(CPACK_PACKAGE_CONTACT "fatkaj@ntis.zcu.cz")

if (WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Cell simulator")
    #set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
    #set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
    #set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif (APPLE)
    configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in "${PROJECT_BINARY_DIR}/Info.plist" @ONLY)
    set(CPACK_BUNDLE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/icons/icon.icns")
    set(CPACK_BUNDLE_PLIST "${PROJECT_BINARY_DIR}/Info.plist")
    set(CPACK_BUNDLE_STARTUP_COMMAND "${CMAKE_SOURCE_DIR}/bundle.sh")
else ()
    set(CPACK_STRIP_FILES "bin/cell-sim-gui")
    set(CPACK_SOURCE_STRIP_FILES "")
    # autogenerate dependency information
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif ()

set(CPACK_PACKAGE_EXECUTABLES "Cell Simulator" "Cell Simulator")

INCLUDE(CPack)

# ######################################################################### #
