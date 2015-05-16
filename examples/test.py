import core
import simulator
import render

# Background color
background = [0, 1, 0, 1]
# Drawable
drawable = render.GridColor()

def parseColor(str):
    return render.Color(1, 0, 0, 1)

#
# Configure module
#
def configure(config):
    background = parseColor(config.getString('background'))

#
# Update module
#
def update(dt, simulator):
    print dt

#
# Prepare module for rendering
#
def drawInit(context):
    drawable.init(context, core.VectorUint(200))

#
# Draw module
#
def draw(context, simulation):
    context.matrixPush()
    context.matrixScale(simulation.getWorldSize())
    drawable.draw(context)
    context.matrixPop()
