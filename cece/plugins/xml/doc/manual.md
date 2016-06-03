
## XML
Basic and most supported loader. It loads simulation from valid *XML* file.
Some basic knowledge of *XML* files is required.
The simplest simulation file looks like:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation dt="1s">
</simulation>
```

This simulation does almost nothing just runs with iteration time step 1 second.

In the simulation element can be several other elements that specifies the simulation.
Those elements can have sub-elements but that mostly depends on element type and used plugin.

### Basic elements:

Simulator core defines following elements and their parameters.

#### Simulation

The *XML* file root element. It must be always present.

| Name                   | Type              | Default              | Description |
| ---------------------- | ----------------- | -------------------- | ----------- |
| `world-size`           | `vector[unit[m]]` | -                    | Simulation world size. |
| `dt`                   | `unit[s]`         | -                    | Simulation time step. |
| `iterations`           | `uint`            | `0`                  | Number of simulation iterations. `0` means unlimited. |
| `background`           | `color`           | `white`              | Background color. |
| `text-color`           | `color`           | `background` inverse | Color of UI text. |
| `text-size`            | `uint`            | `30`                 | UI text size. |
| `show-simulation-time` | `bool`            | `false`              | If simulation time should be rendered. |

#### Init

This element defines program that is executed before the simulation is started. It's good for simulation scene
initialization in cases when manual specification is hard to write.

| Name       | Type          | Default | Description |
| ---------- | ------------- | ------- | ----------- |
| `language` | `plugin-name` | -       | Language in element content. |

#### Plugin

Explicitly loads required plugin. Plugin is implicitly loaded and when is needed but in some cases there is need for explicit load and configuration.

| Name   | Type          | Default | Description |
| ------ | ------------- | ------- | ----------- |
| `name` | `plugin-name` | -       | Plugin name. |

> Additional attributes are passed to plugin configuration.

#### Module

Adds module to simulation.

| Name   | Type                        | Default | Description |
| ------ | --------------------------- | ------- | ----------- |
| `name` | `plugin-name(.module-name)` | -       | Name of required module. In most cases you can specify only plugin name but some plugins offsers more modules so you can specify which one you want by adding a suffix. |

> Additional attributes are passed to module configuration.

#### Program

Defines a program that is available for objects. Programs are not executed until are assigned to objects.

| Name       | Type          | Default | Description |
| ---------- | ------------- | ------- | ----------- |
| `name`     | `string`      | -       | Unique program name. |
| `language` | `plugin-name` | -       | Language in which is the program written. |

#### Object

Adds an object into simulation. Objects are physical entities that can be affected during simulation.

| Name       | Type                | Default | Description |
| ---------- | ------------------- | ------- | ----------- |
| `class`    | `plugin-name.name`  | -       | Unique program name. |
| `visible`  | `bool`              | `true`  | If object is rendered. |
| `position` | `vector[unit[m]]`   | `0 0`   | Initial object position. `0 0` is in middle of the world. |
| `velocity` | `vector[unit[m/s]]` | `0 0`   | Initial object velocity. |
| `density`  | `unit[g/m3]`        | `1`     | Initial object density. |
| `programs` | `array[string]`     | -       | A list of object programs. |

#### Obstacle

Adds a physical obstacle into simulation. There are 3 different types of obstacles. Each of them require different attributes.

| Name       | Type                | Default | Description |
| ---------- | ------------------- | ------- | ----------- |
| `visible`  | `bool`              | `true`  | If object is rendered. |
| `position` | `vector[unit[m]]`   | `0 0`   | Initial object position. `0 0` is in middle of the world. |
| `type`     | `string`            | -       | Type of obstacle. Possible values: `circle`, `rectangle`, `polygon`. |
| `radius`   | `unit[m]`           | -       | Circle radius. Required for `circle` type. |
| `size`     | `vector[unit[m]]`   | -       | Rectangle size. Required for `rectangle` type. |
| `vertices` | `array[vector[unit[m]]]` | -  | A list of vertices. Required for `polygon` type. |
