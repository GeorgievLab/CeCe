# ######################################################################### #

# Project name
set(PROJECT_NAME module-diffusion)

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Signal.hpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    SignalGrid.hpp
)

# Build static / dynamic
list(FIND MODULES_BUILDIN "diffusion" IS_STATIC)

if (${IS_STATIC} GREATER -1)
    message("-- Module ${PROJECT_NAME} is build-in")

    if (UNIX)
        add_compile_options(-fPIC)
    endif ()

    add_definitions(-DLIBRARY_BUILDIN=1)
    add_library(${PROJECT_NAME} ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
    )
else ()
    message("-- Module ${PROJECT_NAME} is dynamic")
    add_library(${PROJECT_NAME} SHARED ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
        simulator
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        simulator
        ${Boost_LIBRARIES}
    )

    # Install
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
    )

endif ()

# ######################################################################### #
