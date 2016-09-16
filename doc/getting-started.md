---
layout: article
permalink: /documentation/getting-started
title: "Getting started"
share: false
categories:
    - documentation
---

The first step is download CeCe application. Go to the [download](/download/) page into `Stable builds` section and select build for your platform (currently supported are Windows, macOS/OSX and Linux). If you need or want the newest version you can look to the `Nighty builds` section to obtain lastest CeCe build (you have to select CLI or GUI application), bud expect unstable/untested behaviour.

Extract downloaded archive to wherever you want, application should work everywhere.

> Following text will focus on CLI application which is currently most supported (GUI application is experimental).

For executing CeCe simulation you have to run CLI application from terminal. All supported platforms have some type of terminal preinstalled.

* *Windows* -- `cmd.exe` or `PowerShell`
* *macOS/OSX* -- `Terminal`
* *Linux* -- `bash` or anything `sh` compatible.

Application is stored in `bin` directory and you have to execute it from terminal. Open terminal in extracted archive or navigate to that directory and then run following command which prints application help.

* *Windows* -- `bin\cece-cli.exe --help`
* *macOS/OSX* -- `./bin/cece-cli --help`
* *Linux* -- `./bin/cece-cli --help`

When you have a simulation file (you can obtain one from [examples](https://github.com/GeorgievLab/CeCe-examples)), just pass path to the simulation file as application argument:

* *Windows* -- `bin\cece-cli.exe ..\demo.cece`
* *macOS/OSX* -- `./bin/cece-cli ../demo.cece`
* *Linux* -- `./bin/cece-cli ../demo.cece`

Simulation file may require some plugins to work (plugins do most of the simulation work). Plugins that are available to you for using in simulations can be obtained by calling.

* *Windows* -- `bin\cece-cli.exe --plugins`
* *macOS/OSX* -- `./bin/cece-cli --plugins`
* *Linux* -- `./bin/cece-cli --plugins`

Plugin can offer loaders, modules, object types and other things. More detailed information about plugin can be obtained by calling (plugin `cell`).

* *Windows* -- `bin\cece-cli.exe --plugin cell`
* *macOS/OSX* -- `./bin/cece-cli --plugin cell`
* *Linux* -- `./bin/cece-cli --plugin cell`