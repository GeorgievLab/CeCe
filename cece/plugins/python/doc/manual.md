## Python

Python support for dynamic programming.

> Python plugin doesn't support `unit` data type and `float` type is used instead.

### Programs
Dynamic program generated from Python source code. Called (function `call`) once in each iteration per object that uses this program.

```python
# Optional initialization code

# Required function prototype.
# object: Object that owns this program.
# simulation: Current simulation.
# dt: Current iteration time step.
def call(object, simulation, dt):
    pass
```

### Modules

Dynamic modules generated from Python source code. Module's `update` function is called once in each iteration.

```python
# Optional initialization code

# Optional function prototype
# @param simulator.Simulation    simulation Current simulation.
# @param simulator.Configuration config     Configuration from XML file.
def configure(simulation, config):
    pass
    
# Recommended function prototype
# @param simulator.Simulation simulation Current simulation.
# @param float                dt         Current iteration  time step.
def update(simulation, dt):
    pass
    
# Optional function prototype.
# @param render.Context       context    Rendering context.
# @param simulator.Simulation simulation Current simulation.
def draw(context, simulation):
    pass
```

### Classes

There are some wrappers around simulator core classes.

#### class `simulator.Configuration`

Configuration class.

##### Methods:

| Name  | Return   | Arguments | Description                                   |
| ------| -------- | --------- | --------------------------------------------- |
| `get` | `string` | `string`  | Returns configuration value under given name. |


#### class `simulator.Simulation`

Main simulation class that contains everything about simulation.

### Properties:

| Name           | Type            | Description                  |
| -------------- | --------------- | ---------------------------- |
| `worldSize`    | `vector[float]` | Simulation scene/world size. |
| `iteration`    | `uint`          | Current iteration number.    |
| `iterations`   | `uint`          | Total number of required iterations. Can be `0`, if there is no limitation. |
| `timeStep`     | `float`         | Simulation time step.        |
| `totalTime`    | `float`         | Total time spend in simulation in seconds. |
| `objectsCount` | `uint`          | Number of objects in scene.  |

##### Methods:

| Name          | Return             | Arguments        | Description   |
| ------------- | ------------------ | ---------------- | --------- |
| `useModule`   | `simulator.Module` | `string`         | Returns required module. If module is not used, it is created by with default configuration. |
| `buildObject` | `simulator.Object` | `string`, `bool` | Create a new object. The first argument is class name and second one is if object should be static (non-movable). |

#### class `simulator.Module`

Base class for all modules. It doesn't offer anything.

#### class `simulator.Object`

Basic simulation object.

### Properties:

| Name       | Type            | Description                  |
| ---------- | --------------- | ---------------------------- |
| `id`       | `uint`          | Grid size.                   |
| `position` | `vector[float]` | Number of different signals. |
| `rotation` | `float`         | Object rotation in radians.  |
| `velocity` | `vector[float]` | Object velocity.             |

##### Methods:

| Name         | Return | Arguments | Description                            |
| ------------ | ------ | --------- | -------------------------------------- |
| `useProgram` | -      | `string`  | Add program with given name to object. |

