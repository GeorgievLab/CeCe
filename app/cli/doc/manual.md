
# CLI application

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
| arrows | Scene view move. |
| mouse drag | Scene view move. |
| `I` | Increase zoom level. |
| `O` | Decrease zoom level. |
| mouse scroll | Increase or decrease zoom level. |
| ... | Enable or disable visualization layer - key is dependent on selection in the simulation file. |

### How to run

CLI application is supported on all three platforms.

#### Windows

On Windows the ZIP package contains executable in the main directory and some
subdirectories with examples and plugins. Application must be executed from command line (`cmd` or `PowerShell`).

```
PS > .\bin\cece-cli.exe examples\demo.cece
```

#### Mac OS X

Application on Mac is shipped as bundle packed in TZG package. Package contains a few directories where the most important
is `bin` where the application is stored.

Just run the simulator by typing this in terminal in directory of unpacked TZG package.

```bash
$ ./bin/cece-cli examples/demo.cece
```

#### Linux

We offer only TGZ for Ubuntu-like distributions (Ubuntu 14.10 LTS, Mint 17.3).

In case of TGZ package.

```bash
$ ./bin/cece-cli share/cece/examples/demo.cece
```

