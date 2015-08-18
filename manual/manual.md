# Applications
Simulator offers two application types, CLI and GUI. CLI is usefull when you need run simulation on background or vizualize the final simulation without any special GUI controls. GUI is good for designing and testing.

## CLI
Simple command line application that can run prepared simulations.
It takes simulation file and perform simulation with optional vizualization.

##### Parameters:

| Name              | Parameters | Description                         |
| ----------------- | ---------- | ----------------------------------- |
| `simulation-file` | `filepath` | Path to simulation file.            |
| `--plugins`       |            | Prints a list of available plugins. |
| `--help`          |            | Prints help.                        |
| `--vizualize`     |            | If simulation should be vizualized. |
| `--novizualize`   |            | Don't vizualize simulation.         |
| `--fullscreen`    |            | Start vizualization window in fullscreen. You don't have to specify window width and height. In that case monitor size is used. |
| `--width`         | `int`      | Vizualization window width.         |
| `--height`        | `int`      | Vizualization window height.        |

Specifying `--vizualize` or `--novizualize` override settings from simulation (some simulation don't explicitly want vizualize).

### How to run

CLI application is supported on all three platforms.

#### Windows x64

On Windows the ZIP package contains executable in the main directory and some subdirectories with examples and plugins. Application must be executed from command line (`cmd` or `PowerShell`).

```
PS > .\simulator.exe examples\showcase.xml
```

#### Mac OS X

Application on Mac is shipped as bundle packed in TZG package. Package contains a few directories where the most important is `bin` where the application is stored. The directory `bin/simulator-cli.app` contains everything that CLI application needs to be executed. Run it from `Finder` is not viable (mostly for GUI apps that doesn't require arguments) so it must be executed from terminal. The bundle have some predefined structure where the executable is stored but it's not important because there is `bin/run.sh` that allows you to run CLI app without knowledge of bundle structure.

Just run the simulator by typing this in terminal in directory of unpacked TZG package.

```bash
$ ./bin/run.sh examples/showcase.xml
```

#### Linux x64

Only Ubuntu 14.04 LST and Linux Mint 17.x is supported. Just double click on DEB package and everything should be installed. Then just type following into terminal.

```bash
$ simulator-cli /usr/share/simulator/examples/showcase.xml
```

## GUI
TODO

# Loaders
Loaders are responsible for loading different type of source files.
They understand the source file and create simulation from them.

## XML
Basic and most supported loader. It loads simulation from valid XML file.
Some basic knowledge of XML files is required.
The simplest simulation file looks like:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation dt="1s">
</simulation>
```

This simulation does almost nothing just runs with iteration time step 1 second.

In the simulation element can be several other elements that specifies the simulation.
Those elements can have sub-elements but that mostly depends on element type and used plugin.

##### Basic elements:

| Name | Attributes | Description | Content |
| ---- | ---------- | ----------- | ------- |
| `parameter` | `name:string`, `value:expression` | Simulation parameter. | |
| `init` | `language:plugin-name` | Simulation iteration step. | Source code of plugin initializer interpreter. |
| `plugin` | `name:plugin-name` | Load required plugin. Some plugins might install some hooks (see `python` plugin). |
| `module` | `name:plugin-name(.string)`, ... | Create a module from given plugin. Module name can be specified in case the plugin offers more than one module. | Depends on module. |
| `program` | `name:string`, `language:plugin-name` | Defines simulation program that can be used by objects under given name. | Program source code that is understandable by plugin/language. |
| `object` | `class:plugin-name.name`, `position:vector[unit[m]]`, `density:unit[g/m3]`, `programs:names`, `static:bool`, ... | Create an object in simulation. | Depending on object class. |
| `obstacle` | `type:string`, `position:vector[unit[m]]`, ... | Setup an obstacle. | |

Attributes in this table is in form where the part before colon is attribute name and the part after is value type description.

##### Value types:

| Name          | Description           | Example |
| ------------- | --------------------- | ------- |
| `int`         | Integer value.        | `5`, `-3` |
| `uint`        | Unsigned integer value. | `5` |
| `float`       | Floating point value. | `5.3` |
| `string`      | String value.         | `hello world` |
| `expression`  | Expression. Can contains parameters that are defined by parameter element. | `10 * 5 + 3 * sin(alpha)` |
| `plugin-name` | Name of existing plugin. | `cell` |
| `name`        | Similar to `string`.  | `print` |
| `unit`        | SI unit based on specified symbol in square braces. Accepts value without unit symbol but if symbol is specified it must match the unit type (prefix are supported). Value without symbol have unspecified size, but mostly corresponds to basic unit specified by simulator (`um`, `s`, `ng`). | `0`, `30um/s`, `1.3m/s2`, `0.01/s`, <del>`5um/us`</del> |
| `vector`      | Value of **two** values separated by space. In case the second value is not supplied it is same as the first one. | `10um/s 1um/s`, `0 0` |
| `color`       | Color value. Value can be name of predefined color or in CSS color definition format (#RRGGBB). | `red`, `#0000FF` |


## Reactions
This loader loads simple files that contains reaction rules and some parameters.
Syntax of reaction rules is same as stochastic-reaction plugins.

```
@iterations 500
@dt 1s
null > 0.3 > A;
A > 0.1 > B + C;
```

Lines that begins with `@` symbol are considered as directives that setups simulation.
Remaining lines are parsed by reactions parser and used as program for cell.

Coresponding XML file looks like:
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation dt="1s" iterations="500">
    <program name="__local__" language="stochastic-reactions-intercellular">
<![CDATA[
        null > 0.3 > A;
        A > 0.1 > B + C;
]]>
    </program>
    <object class="cell.Yeast" programs="__local__" />
</simulation>
```

List of available directives:

| Directive    | Parameters             | Description                           | Example           |
| ------------ | ---------------------- | ------------------------------------- | ----------------- |
| `iterations` | `int`                  | Number of simulation iterations.      | `@iterations 100` |
| `dt`         | `time`                 | Simulation iteration step.            | `@dt 10s`         | 
| `molecule`   | `string`, `int`        | Initial amount of specified molecule. | `@molecule A 500` |
| `parameter`  | `string`, `expression` | Set simulation parameter that can be used in reaction rules. | `@parameter kA 0.3` |

# Data Tables
Data tables are way how the simulator stores data within simulation. Anything can store data in named data table and when simulation ends those data tables are stored into *CSV* files in current working directory.

# Plugins
List of available plugins supplied within official distribution.

| Name                                 | Plugin name                          | Description |
| ------------------------------------ | ------------------------------------ | ----------- |
| Agglutination                        | `agglutination`                      | Agglutination support between simulation objects/cells. |
| Cell                                 | `cell`                               | Biological cells support. |
| Cell Python                          | `cell-python`                        | Python support for `cell` plugin. |
| Diffusion                            | `diffusion`                          | Diffusion.
| Diffusion Python                     | `diffusion-python`                   | Python support for `diffusion` plugin. |
| Diffusion Streamlines                | `diffusion-streamlines`              | Diffusion and Streamlines connector. |
| Object generator                     | `object-generator`                   | Object generator.                   |
| Picture                              | `picture`                            | Picture storing support.            |
| Python                               | `python`                             | Basic Python support.               |
| Stochastic reactions - Intercellular | `stochastic-reactions-intercellular` | Intercellular stochastic reactions. |
| Stochastic reactions - Intracellular | `stochastic-reactions-intracellular` | Intracellular stochastic reactions. |
| Streamlines                          | `streamlines`                        | Streamlines computed by using Lattice Boltzman. |
| Streamlines Python                   | `streamlines-python`                 | Python support for `streamlines` plugin. |

## Agglutination (WIP)
This plugin enables simulation object sticking. When two simulation object collides they migth be bound together and after some time the bound can be removed.

## Cell
Plugin offers things usefull to working with cells.

### Objects

Objects offered by `cell` plugin:

| Name            | Description         |
| --------------- | ------------------- |
| `cell.Cell`     | Common cell object. |
| `cell.Yeast`    | Yeast cell.         |

All object share common (unavailable) ancestor that have following properties:

| Property      | Units           | Description       | Example  |
| ------------- | --------------- | ----------------- | -------- |
| `volume`      | m<sup>3</sup>   | Cell volume       | `300um3` |
| `growth-rate` | m<sup>3</sup>/s | Cell growth rate. | `3um3/s` |

Cell can have set initial amount of molecule at the beginning.

```xml
<object class="cell.Yeast>
  <molecule name="GFP" amount="500" />
</object>
```

### Programs
#### `store-molecules`
Program that stores amount of all molecules in current iteration to `molecules` data table. It stores object identifier to distinguish between multiple cells.

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `id`        - Object identifier.
* `...`       - Column given by molecule name and value is amount of that molecule.

## Cell Python
Adds access to `cell` plugin objects from Python.

### Objects

| Name            | Parent             | Description        |
| --------------- | ------------------ | ------------------ |
| `cell.BaseCell` | `simulator.Object` | Base cell object.  |
| `cell.Yeast`    | `cell.BaseCell`    | Yeast cell.        |

#### Object `cell.BaseCell`

##### Properties:

| Name         | Type    | Description       |
| ------------ | ------- | ----------------- |
| `volume`     | `float` | Cell volume       |
| `growthRate` | `float` | Cell growth rate. |

##### Methods:

| Name            | Return  | Arguments | Description       |
| --------------- | ------- | --------- | ----------------- |
| `moleculeCount` | `float` | `string`  | Return amount of required molecule stored in cell. |

## Diffusion
  
The main module adds support for global diffusion in simulation. Diffusion can handle any number of separate signals. They cannot affect each other.

##### Example:

Enables diffusion with two signals named *GFP* and *RFP* with diffusion and degradation rates. Those signals have specified colors how they are rendered.

```xml
<module name="diffusion" grid="100">
  <signal name="GFP" diffusion-rate="30um2/s" degradation-rate="0.1/s" color="green" />
  <signal name="RFP" diffusion-rate="5um2/s" color="red" />
</module>
```

##### Parameters:

| Name   | Type           | Default        | Description |
| ------ | -------------- | -------------- | ----------- |
| `grid` | `vector[uint]` | -              | Diffusion grid size. |

### Signals

You can specify any number of different signals, there is no limitation. Each signal is specified separately.

##### Parameters:

| Name               | Type           | Default        | Description |
| ------------------ | -------------- | -------------- | ----------- |
| `name`             | `string`       | -              | Signal name. |
| `diffusion-rate`   | `unit[m2/s]`   | -              | Diffusion rate. |
| `degradation-rate` | `unit[/s]`     | `0/s`          | Degradation rate. |
| `color`            | `color`        | Predefined     | Signal color when the module is rendered. |

### Additional modules

#### `store-state`

Module that stores values from signal grid of all signals into `diffusion` data table.

## Diffusion Python

Python language binding for `diffusion` plugin.

##### Example:

```xml
<module name="python:generate"><![CDATA[
import math

def update(dt, simulation):
    diffusion = simulation.useModule("diffusion")
    size = diffusion.gridSize
    radius = size.x / 20
    signalAmount = 1

    for x in range(-radius, radius + 1):
        for y in range(-radius, radius + 1):
            if (math.sqrt(x * x + y * y) <= radius):
                diffusion.setSignal(0, size.x / 2 + x, size.y / 2 + y, signalAmount)
]]></module>
```

### Class `diffusion.Module`

Diffusion module wrapping class.

##### Properties:

| Name          | Type           | Description                  |
| ------------- | -------------- | ---------------------------- |
| `gridSize`    | `vector[uint]` | Grid size.                   |
| `signalCount` | `uint`         | Number of different signals. |

##### Methods:

| Name        | Return  | Arguments                       | Description                                               |
| ------------| ------- | ------------------------------- | --------------------------------------------------------- |
| `getSignal` | `float` | `string`, `int`, `int`          | Return signal value of given signal at given coordinates. |
| `setSignal` | -       | `string`, `int`, `int`, `float` | Change signal value of given signal at given coordinates. |

## Diffusion Streamlines

Without this plugin/module the streamlines does not affect diffusion. Just simply add:

```xml
<module name="diffusion-streamlines" />
```

> It doesn't require from diffusion and streamlines module to have same grid sizes.

## Object generator
Module that generates specified object with some probability and parameters.

##### Example:

This example create module that generates different Yeast cells in each iteration with some probability.

```xml
<module name="object-generator">
  <object class="cell.Yeast" probability="0.004" programs="make-gfp" position-min="-80um -30um" position-max="-78um 30um" />
  <object class="cell.Yeast" probability="0.004" programs="make-rfp" position-min="-80um -30um" position-max="-78um 30um" />
  <object class="cell.Yeast" probability="0.004" />
  <object class="cell.Yeast" probability="0.005" />
</module>
```

### Objects

##### Parameters:

| Name           | Type              | Default        | Description |
| -------------- | ----------------- | -------------- | ----------- |
| `class`        | `name`            | -              | Generated object class. |
| `probability`  | `unit[]`          | -              | Probability of object spawning. |
| `programs`     | `names ...`       | -              | Space separated list of program names. |
| `position-min` | `vector[unit[m]]` | Left side of simulation scene. | Minimum position where object can be spawned. |
| `position-max` | `vector[unit[m]]` | Left side of simulation scene. | Maximum position where object can be spawned. |

## Picture
Module that can store scene picture into file.

##### Example:

Stores scene picture each 100 iterations into file with name `pic_$1.png` in directory `pictures`.

```xml
<module name="picture" pattern="pictures/pic_$1.png" iteration="100" />
```

##### Parameters:

| Name        | Type     | Default        | Description |
| ----------- | -------- | -------------- | ----------- |
| `pattern`   | `string` | `image_$1.png` | Path to output file. Pattern can have special `$1` sequence that is replaced by iteration number. Currently is supported only *PNG* files. |
| `iteration` | `int`    | `1`            | Number of iteration to skip before store picture (~ store each Nth iteration). |

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


## Stochastic reactions - Intracellular

Allows to specify reactions rules that are stochasticaly being executed inside the cell.

They syntax is similiar to one in NFSim.

This reaction changes molucule A to molucule B with rate 1.
In other words, when this reaction occurs, one molecule A gets substracted and one molecule B gets added.

A > 1 > B;

This reaction changes one molucule A into molucules B and C.
A > 1 > B + C;

If you need reaction to happen only when some other molecule is present, take a look at following reaction.
This reaction subtracts A and B, and adds C + B. Therefore, this raction changes A to C only when B is present.
Please note you cant use this concept with environmental reactions.
A + B > 3 > C + B;

This reaction creates complex C from 2 molucules of A.
That means that at least two As are required for this reaction to occur.
A + A > 2 > C;      
                    
This reaction uses keyword "null" and represents expression of A.
No molecule gets substracted and one molucule of A gets added.
null > 2 > A;

Similiarly, this reaction repressents degradation of A.
A > 5 > null;

Those two reactions can be easily rewritten like this using reversible reaction syntax.
First rate is rate of reaction going back, and second rate is for reaction going forward.
A < 5, 2 > null;
null < 2, 5 > A;

Another keyword which helps user to make his reaction rules more understandable is "if" "and" and "or".
if not C and D and E:  A + B > 1 > C;
This reaction merges A and B into C, but this reaction can occur only when there is no molucule of C present and simultaneously there must be D and E present.
You can combine as many logic combinations as you can.
Please note that "and" is prior to "or", this means that following reaction only occur when there is either B and C together or A present in the cell.
if A or B and C: D > 1 > E;

##### Example:

Defines reactions that create GFP molecule with some degradation.

```
<program name="make-gfp" language="stochastic-reactions-intracellular"><![CDATA[
  null > 1.505149978 > GFP;
  GRP > 0.001 > null;
]]></program>
```

## Stochastic reactions - Intercellular

Allows to specify reactions program that happens inside cell and surrounding environment. Plugin is just an extension of intracellular reactions.

The extension lies in keywork "env" - environment, which handles absorbing the molucules inside the cell or prodicing them outside.
Please note that env must be alone on its side of reaction rule.

> Plugin requires properly set `diffusion` module with signals that have same name as molecules released into environment.

##### Example:

This example is production of A molecules and releasing them with some rate into diffusion.

```
<program name="make-gfp" language="stochastic-reactions-intercellular"><![CDATA[
  null < 0.001, 50 > A;
  A > 0.9 > env;
]]></program>
```

## Streamlines

Module that simulate streamlines in whole scene. It using Lattice Boltzman method to calculate velocities in simulation grid. Simulation can handle static and even dynamic obstacles. Dynamic obstacles are moving objects in scene. Dynamic obstacles (i.e. objects) are affected by computed velocities.

There are some limitations that come from Lattice Boltzman method. Maximum velocity is limited by grid size, time step and scene/world size. If the velocity is higher than this limit, it's changed to the maximum otherwise the streamlines simulation crashes.

##### Example:

```xml
<module name="streamlines" grid="200" velocity-inflow="300um/s" kinematic-viscosity="60.58um2/s" />
```

##### Parameters:

| Name                  | Type           | Default | Description |
| --------------------- | -------------- | --------| ----------- |
| `grid`                | `vector[uint]` | -       | Simulation grid size. |
| `velocity-inflow`     | `unit[m/s]`    | -       | Inflow velocity. |
| `kinematic-viscosity` | `unit[m2/s]`   | `1`     | Fluid kinematic viscosity. |

### Additional modules
#### `store-state`
Module that stores velicities from grid into `streamlines` data table.

#### `store-object-state`
Module that stores object position and velicities into `object-state` data table.

## Streamlines Python (WIP)

TODO
