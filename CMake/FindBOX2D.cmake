#
# Find Box2D library.
#
# Input (optional):
#   BOX2D_STATIC_LIB  - If static libraries should be used.
#   BOX2DDIR          - Root directory.
#
# Output:
#   BOX2D_INCLUDE_DIR - Path to include directories.
#   BOX2D_LIBRARIES   - List of libraries.
#   BOX2D_FOUND       - True if Box2D was found.
#

if (BOX2D_INCLUDE_DIR)
    # Already in cache, be silent
    set(BOX2D_FIND_QUIETLY TRUE)
endif (BOX2D_INCLUDE_DIR)

# Find header files
find_path(BOX2D_INCLUDE_DIR Box2D/Box2D.h
    PATH_SUFFIXES
        include
    PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw          # Fink
        /opt/local   # DarwinPorts
        /opt/csw     # Blastwave
        /opt
        ${BOX2DDIR}
        $ENV{BOX2DDIR}
        "$ENV{PROGRAMFILES}/Box2D"
)

set(BOX2D_LIBRARY_NAME Box2D)

if (BOX2D_STATIC_LIB)
    set(BOX2D_LIBRARY_NAME libBox2D.a Box2D.lib)
endif ()

# Find library
find_library(BOX2D_LIBRARY
    ${BOX2D_LIBRARY_NAME}
    PATH_SUFFIXES
        lib64
        lib
    PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw          # Fink
        /opt/local   # DarwinPorts
        /opt/csw     # Blastwave
        /opt
        ${BOX2DDIR}
        $ENV{BOX2DDIR}
        ${BOX2DDIR_LIB}
        $ENV{BOX2DDIR_LIB}
        "$ENV{PROGRAMFILES}/Box2D"
)

set(BOX2D_FOUND FALSE)

if (BOX2D_LIBRARY)
    # Library found
    set(BOX2D_FOUND TRUE)
endif ()

# Handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOX2D DEFAULT_MSG BOX2D_LIBRARY BOX2D_INCLUDE_DIR)

if (BOX2D_FOUND)
    set(BOX2D_LIBRARIES ${BOX2D_LIBRARY})
else ()
    set(BOX2D_LIBRARIES)
endif ()

mark_as_advanced(
    BOX2D_FOUND
    BOX2D_INCLUDE_DIR
    BOX2D_LIBRARY
)

# ######################################################################### #
