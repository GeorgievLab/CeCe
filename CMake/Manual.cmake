# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

option(CONFIG_MANUAL_CREATE "Create user manual" On)

if (NOT CONFIG_MANUAL_CREATE)
    set(CONFIG_MANUAL_EXTERN "CONFIG_MANUAL_EXTERN-NOTFOUND" CACHE STRING "Path to extern manual (this allow to create manual on PC where pandoc is installed without installing it on current PC)")
endif ()

# ######################################################################### #

if (CONFIG_MANUAL_CREATE)
    # Find build program
    find_program(PANDOC pandoc)

    if (NOT PANDOC)
        message(WARNING "pacdoc not found, cannot create manual")
        set(CONFIG_MANUAL_CREATE Off)
    endif ()
endif ()

# ######################################################################### #

if (CONFIG_MANUAL_CREATE)
    file(GLOB LOADER_FILES manual/loaders/*.md)
    file(GLOB PLUGIN_FILES manual/plugins/*.md)

    LIST(SORT LOADER_FILES)
    LIST(SORT PLUGIN_FILES)

    set(MANUAL_DIR ${CMAKE_BINARY_DIR}/manual)

    # Create directory for manual
    file(MAKE_DIRECTORY ${MANUAL_DIR})

    # Create manual
    execute_process(COMMAND ${PANDOC}
        -s
        --highlight-style pygments
        -c github.css
        ${CMAKE_SOURCE_DIR}/manual/manual.md
        ${LOADER_FILES}
        ${PLUGIN_FILES}
        -o ${MANUAL_DIR}/manual.html
    )

    # Copy manual CSS to build dir
    file(COPY ${CMAKE_SOURCE_DIR}/manual/github.css DESTINATION ${MANUAL_DIR})

    # Install manual
    install(FILES
        ${MANUAL_DIR}/manual.html
        ${MANUAL_DIR}/github.css
        COMPONENT Manual
        DESTINATION "${INSTALL_DIR_MANUAL}"
    )
elseif (CONFIG_MANUAL_EXTERN)
    # Install extern manual
    install(FILES
        ${CONFIG_MANUAL_EXTERN}
        ${MANUAL_DIR}/github.css
        COMPONENT Manual
        DESTINATION "${INSTALL_DIR_MANUAL}"
    )
else ()
    message(WARNING "Build without manual")
endif ()

# ######################################################################### #
