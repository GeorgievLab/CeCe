
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

