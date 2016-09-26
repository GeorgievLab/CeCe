---
layout: article
permalink: /download/
title: "Download"
share: false
---

## Stable builds

Stable builds contain whole package (CLI application, plugins and examples).

| Version | Release date | OS | Download |
| ------- | ------------ | -- | -------- |{% for link in site.data.build_stable %}
| {{ link.version }} | {{ link.date }} | {{ link.os }} | [<i class="fa fa-download" aria-hidden="true"></i> Download]({{ link.link }}) |{% endfor %}

> If you want to use python plugins you also need python 2.7 installed. On macOS/OSX and Linux there should be already installed but on Windows you need to install it ([64bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.amd64.msi) for 0.6 and above and [32bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.msi) for 0.5)

## CI builds

CI builds are built regulary by CI services ([Travis-CI](https://travis-ci.org), [Appveyor](https://www.appveyor.com)) and stored on [Bintray](https://bintray.com). Those builds are not tested and can contain bugs but on other hand they offer improvements in comparision with stable builds.

### CLI

Command line application. Usage requires some basic knowledge about command line / terminal. You can look into [documentation](/documentation/) for more information.

| OS | Latest | Older |
| -- | -------- |{% for link in site.data.build_cli_ci %}
| {{ link.os }} | [<i class="fa fa-download" aria-hidden="true"></i> Download]({{ link.link }}) | [<i class="fa fa-download" aria-hidden="true"></i> Download]({{ link.all }}) | {% endfor %}

## Source code

Whole project is released as open-source under [GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html) license and source is available on [GitHub](https://github.com). Project is split into separate parts which can be used separately. You can use only core library and plugins and create own version of CLI or GUI application.

* [Core library](https://github.com/GeorgievLab/CeCe-cli)
* [Standard plugins](https://github.com/GeorgievLab/CeCe-plugins)
* [Examples](https://github.com/GeorgievLab/CeCe-examples)
* [CLI application](https://github.com/GeorgievLab/CeCe-cli)
* [GUI application](https://github.com/GeorgievLab/CeCe-gui)
