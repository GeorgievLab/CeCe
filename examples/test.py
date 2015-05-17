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
    for i in xrange(1, 201):
        for j in xrange(1, 201):
            if i == j:
                drawable.set(core.VectorUint(i, j), render.Color(1, 0, 0, 0))
            else:
                drawable.set(core.VectorUint(i, j), render.Color(0, 0, 0, 0))

#
# Prepare module for rendering
#
def drawInit(context):
    drawable.init(context, core.VectorUint(200))
    drawable.clear(render.Color(0))

#
# Draw module
#
def draw(context, simulation):
    context.matrixPush()
    context.matrixScale(simulation.worldSize)
    drawable.draw(context)
    context.matrixPop()
