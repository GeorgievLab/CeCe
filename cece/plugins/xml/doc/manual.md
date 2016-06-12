
# XML plugin

Basic and most supported simulation loader. It loads simulation from valid *XML* file, so
some basic knowledge of *XML* files is required.

The simplest simulation file looks like:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation dt="1s">
</simulation>
```

This simulation does almost nothing just runs with iteration time step 1 second.

In the simulation element can be several other elements that specifies the simulation.
Those elements can have sub-elements but that mostly depends on element type.

## Basic elements:

CeCe core defines following elements and their parameters:

* `simulation`
* `plugin`
* `init`
* `module`
* `object`
* `program`
* `type`

### Simulation element

The *XML* file root element. It must be always present.

| Name                   | Type              | Default              | Description |
| ---------------------- | ----------------- | -------------------- | ----------- |
| `world-size`           | `vector[unit[m]]` | -                    | Simulation world size. |
| `dt`                   | `unit[s]`         | -                    | Simulation time step. |
| `iterations`           | `uint`            | `0`                  | Number of simulation iterations. `0` means unlimited. |
| `background`           | `color`           | `white`              | Background color. |
| `length-coefficient`   | `real`            | `1`                  | Coefficient for conversion between simulation length and physics engine length. Used physics engine have some limitation. |

### Plugin element

Element that explicitly import required plugin. Objects from plugin that is not imported cannot be used in simulation.

| Name   | Type          | Default | Description |
| ------ | ------------- | ------- | ----------- |
| `name` | `plugin-name` | -       | Required plugin name. |

> It's possible to specify additional element attributes that will be passed to
> plugin configuration. See more in specific plugin manual.

### Init element

This element defines program that is executed before the simulation is started.
It's good for simulation scene initialization in cases when manual specification
is hard to write.

| Name       | Type          | Default | Description |
| ---------- | ------------- | ------- | ----------- |
| `type`     | `plugin-name` | -       | Type of used initializer. |
| `language` | `plugin-name` | -       | Language in element content (same as `type`). |

> Additional attributes can be specified and will be passed to initializer in configuration step.

### Module element

Adds module to simulation.

| Name   | Type               | Default | Description |
| ------ | ------------------ | ------- | ----------- |
| `name` | `module-type-name` | -       | Name of required module. Modules that are available for simulation is taken from imported plugins. |

> Additional attributes can be specified and will be passed to module in configuration step.

### Program element

Defines a program that is available for objects. Programs are not executed until are assigned to objects.

| Name       | Type                | Default | Description |
| ---------- | ------------------- | ------- | ----------- |
| `name`     | `string`            | -       | Unique program name under which will be available for simulation objects. |
| `type`     | `program-type-name` | -       | Program type. |
| `language` | `program-type-name` | -       | Language in which is the program written (same as `type`). |

> Additional attributes can be specified and will be passed to program in configuration step.

### Object element

Adds an object into simulation. Objects are physical entities that can be affected during simulation.

| Name       | Type                | Default | Description |
| ---------- | ------------------- | ------- | ----------- |
| `class`    | `object-type-name`  | -       | Unique program name. |
| `visible`  | `bool`              | `true`  | If object is rendered. |
| `position` | `vector[unit[m]]`   | `0 0`   | Initial object position. `0 0` is in middle of the world. |
| `velocity` | `vector[unit[m/s]]` | `0 0`   | Initial object velocity. |
| `density`  | `unit[g/m3]`        | `1`     | Initial object density. |
| `programs` | `array[string]`     | -       | A list of object programs. |

> Additional attributes can be specified and will be passed to object in configuration step.

### Type element

Allows to define custom object type.

| Name   | Type               | Default | Description |
| ------ | ------------------ | ------- | ----------- |
| `name` | `name`             | -       | Name of custom defined object type. |
| `type` | `object-type-name` | -       | Name of object type that is used as base type (cannot be user defined type). |
