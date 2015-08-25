
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

