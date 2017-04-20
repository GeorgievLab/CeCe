---
layout: article
permalink: /download/
title: "Download"
share: false
---

## Stable builds

Stable builds contain whole package (CLI application, plugins and examples).

| Version | Release date | OS | Download |
| ------- | ------------ | -- | -------- |{% for link in site.data.build_stable %}{% if link.lastest == true %}
| {{ link.version }} | {{ link.date }} | {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="Stable"><i class="fa fa-download" aria-hidden="true"></i> Download</a> |{% endif %}{% endfor %}

> If you want to use python plugins you also need python 2.7 installed. On macOS/OSX and Linux there should be already installed but on Windows you need to install it according to the selected architecture: [64bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.amd64.msi) or [32bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.msi).
>
> If python is not installed or wrong version is installed you get warning messages for plugins using python and when you try use one of those plugins you get an error message.

> You can download newer version of plugins separately and replace the original ones. This is handy in cases when a bug doesn't allow you to run your simulation. Just scroll down to *CI builds* *Plugins*, click on your required platform and download the lastest one. Extract the package and copy files from `lib` (`bin` on Windows) to same directory in your simulator directory (it should replace the original files).

### Older versions

| Version | Release date | OS | Download |
| ------- | ------------ | -- | -------- |{% for link in site.data.build_stable %}{% if link.lastest == false %}
| {{ link.version }} | {{ link.date }} | {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="Old"><i class="fa fa-download" aria-hidden="true"></i> Download</a> |{% endif %}{% endfor %}

## CI builds

CI builds are built regulary by CI services ([Travis-CI](https://travis-ci.org), [Appveyor](https://www.appveyor.com)) and stored on [Bintray](https://bintray.com). Those builds are not tested and can contain bugs but on other hand they offer improvements in comparision with stable builds.

### CLI

Command line application. Usage requires some basic knowledge about command line / terminal. You can look into [documentation](/documentation/) for more information.

| OS | Latest | Older |
| -- | -------- |{% for link in site.data.build_cli_ci %}
| {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="CLI"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | <a href="{{ link.all }}" class="download" data-category="CLI"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | {% endfor %}

### GUI

Experimental GUI application.

| OS | Latest | Older |
| -- | -------- |{% for link in site.data.build_gui_ci %}
| {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="GUI"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | <a href="{{ link.all }}" class="download" data-category="GUI"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | {% endfor %}

### Plugins

Packages contain the lastest version of CeCe standard plugins which might not be in CLI application package.

| OS | Latest | Older |
| -- | -------- |{% for link in site.data.build_plugins_ci %}
| {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="Plugins"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | <a href="{{ link.all }}" class="download" data-category="Plugins"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | {% endfor %}

### Plugins - Experimental

Package with experimental plugins which might work, work only partially or not work at all. Use with caution.

| OS | Latest | Older |
| -- | -------- |{% for link in site.data.build_plugins_ci_experimental %}
| {{ link.os }} | <a href="{{ link.link }}" class="download" data-category="Plugins experimental"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | <a href="{{ link.all }}" class="download" data-category="Plugins experimental"><i class="fa fa-download" aria-hidden="true"></i> Download</a> | {% endfor %}

## Source code

Whole project is released as open-source under [GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html) license and source is available on [GitHub](https://github.com). Project is split into separate parts which can be used separately. You can use only core library and plugins and create own version of CLI or GUI application.

* [Core library](https://github.com/GeorgievLab/CeCe-cli)
* [Standard plugins](https://github.com/GeorgievLab/CeCe-plugins)
* [Examples](https://github.com/GeorgievLab/CeCe-examples)
* [CLI application](https://github.com/GeorgievLab/CeCe-cli)
* [GUI application](https://github.com/GeorgievLab/CeCe-gui)

<script>
$("a.download").click(function () {
    var self = $(this);
    ga('send', 'event', {
        'eventCategory': self.data('category'),
        'eventAction': 'Download',
        'eventLabel': self.prop('href')
    });
});
</script>