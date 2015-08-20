# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (CONFIG_RENDER_ENABLE)
    option(CONFIG_PLUGIN_diffusion_SMOOTH "Render smooth diffusion" On)
endif ()

# ######################################################################### #

if (CONFIG_PLUGIN_diffusion_SMOOTH)
    add_definitions(-DCONFIG_PLUGIN_diffusion_SMOOTH=1)
endif ()

# ######################################################################### #

# Sources
set(SRCS
    Library.cpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    StoreState.hpp
    StoreState.cpp
)

# ######################################################################### #

# Build plugin
build_plugin(diffusion
    SOURCES ${SRCS}
)

# ######################################################################### #
