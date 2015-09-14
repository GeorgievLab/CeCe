% The CeCe simulator
% Georgiev Lab (c) 2015

The CeCe simulator is modular simulator primary designed for modeling signal
transmission networks in microfluidics. The simulator architecture is mainly modular
where functionality is provided by plugins. Each plugin is responsible for specific
part of simulation (may not be used).

# Applications

### CLI

Simple command line application that can run prepared simulations.
It takes simulation file and perform simulation with optional visualization.

##### Parameters:

| Name              | Parameters | Description                         | Notes |
| ----------------- | ---------- | ----------------------------------- | ----- |
| `simulation-file` | `filepath` | Path to simulation file.            |       |
| `--plugins`       |            | Prints a list of available plugins. |       |
| `--help`          |            | Prints help.                        |       |
| `--visualize`     |            | If simulation should be visualized. |       |
| `--novisualize`   |            | Don't visualize simulation.         |       |
| `--fullscreen`    |            | Start visualization window in fullscreen. You don't have to specify window width and height. In that case monitor size is used. |       |
| `--width`         | `int`      | Visualization window width.         |       |
| `--height`        | `int`      | Visualization window height.        |       |
| `--capture`       | `filepath` | Record video from simulation. Visualization is required. | May not be available in some builds. Check `--help` if is supported. |

Specifying `--visualize` or `--novisualize` override settings from simulation (some simulation don't explicitly want visualize).

##### Keys

When CLI application is running with visualization some keys can be used.

| Key | Description        |
| --- | ------------------ |
| `Q` | Exits application. |
| `P` | Pause simulation.  |
| `S` | Perform one simulation step when simulation is paused. |

### How to run

CLI application is supported on all three platforms.

#### Windows x64

On Windows the ZIP package contains executable in the main directory and some
subdirectories with examples and plugins. Application must be executed from command line (`cmd` or `PowerShell`).

```
PS > .\cece-cli.exe examples\showcase.xml
```

#### Mac OS X

Application on Mac is shipped as bundle packed in TZG package. Package contains a few directories where the most important
is `bin` where the application is stored. The directory `bin/simulator-cli.app` contains everything that
CLI application needs to be executed. Run it from `Finder` is not viable (mostly for GUI apps that doesn't require arguments)
so it must be executed from terminal. The bundle have some predefined structure where the executable is stored but it's not
important because there is `bin/cece-cli.sh` that allows you to run CLI app without knowledge of bundle structure.

Just run the simulator by typing this in terminal in directory of unpacked TZG package.

```bash
$ ./bin/cece-cli.sh examples/showcase.xml
```

#### Linux x64

We offer only DEB package for Ubuntu-like distributions (Ubuntu 14.10 LTS, Mint 17.2).
Just double click on DEB package and everything should be installed. Then just type following into terminal.

```bash
$ cece-cli /usr/share/cece/examples/showcase.xml
```

# Data Types

In following text there are several data types that have different meaning. You can find their meaning in following table:

| Name          | Description                                    | Example |
| ------------- | ---------------------------------------------- | ------- |
| `int`         | Integer value.                                 | `5`, `-3` |
| `uint`        | Unsigned integer value.                        | `5` |
| `float`       | Floating point value.                          | `5.3` |
| `string`      | String value.                                  | `hello world` |
| `expression`  | Expression. Can contains parameters that are defined by parameter element. | `10 * 5 + 3 * sin(alpha)` |
| `plugin-name` | Name of existing plugin.                       | `cell` |
| `name`        | Similar to `string`.                           | `print` |
| `unit[?]`     | SI unit based on specified symbol in square braces. Accepts value without unit symbol but if symbol is specified it must match the unit type (prefix are supported). Value without symbol have unspecified size, but mostly corresponds to basic unit specified by simulator (`um`, `s`, `ng`). | `0`, `30um/s`, `1.3m/s2`, `0.01/s`, <del>`5um/us`</del> |
| `vector[?]`   | Value of **two** values separated by space. In case the second value is not supplied it is same as the first one. | `10um/s 1um/s`, `0 0` |
| `color`       | Color value. Value can be name of predefined color or in CSS color definition format (#RRGGBB). | `red`, `#0000FF` |
| `it`          | Simulation iteration number. | `10`, `55` |
| `range[?]`    | Range of values. Values can be separated by dash '-'. If dash is not present the meaning is: `X-X`. | `10`, `10-15` |
| `array[?]`    | List of values of same type. | `10 10 -5`, `a b c d e f g` |


# Data Tables
Data tables are way how the simulator stores data within simulation. Anything can
store data in named data table and when simulation ends those data tables are stored
into *CSV* files in current working directory.

> All data is stored in memory during simulation and are stored into file when
simulation ends. This cause a large memory requirements depending on stored data.

<!-- include loaders/* -->

<!-- include plugins/* -->
