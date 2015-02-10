# ######################################################################### #

project(cli)

# Create program
add_executable(cli
    main.cpp
)

# Link libraries
target_link_libraries(cli
    javascript
    simulator
    physics
    v8
    BulletDynamics
    BulletCollision
    LinearMath
)

# Setup dependencies
add_dependencies(cli 
    javascript 
    simulator 
    core 
    physics
)

# ######################################################################### #
