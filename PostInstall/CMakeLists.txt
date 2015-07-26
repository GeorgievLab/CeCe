
if (APPLE)
    # Fixup_bundle command integrate libraries into bundles
    # so there are useless here
    install(CODE "
        execute_process(COMMAND
            rm -rf \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY}\"
        )
    ")
endif ()
