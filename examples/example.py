import core
import simulator
import render

#
# Configure module.
#
# @param simulator.Configuration config Module configuration.
#
def configure(config):
    pass
    
#
# Update module.
#
# @param core.Duration        dt         Time step.
# @param simulator.Simulation simulation Current simulation.
#
def update(dt, simulation):
    pass

#
# Draw module
#
# @param render.Context       context    Rendering context.
# @param simulator.Simulation simulation Current simulation.
#
def draw(context, simulation):
    pass
