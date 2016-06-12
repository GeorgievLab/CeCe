
# Object generator

Module that generates specified object with some probability and parameters.

## Example:

This example create module that generates different Yeast cells in each iteration with some probability.

```xml
<module name="object-generator">
  <object class="cell.Yeast" rate="0.04/s" programs="make-gfp" position-min="-80um -30um" position-max="-78um 30um" />
  <object class="cell.Yeast" rate="0.04/s" programs="make-rfp" position-min="-80um -30um" position-max="-78um 30um" />
  <object class="cell.Yeast" rate="0.04/s">
    <molecule name="GFP" amount="1000" />
  </object>
  <object class="cell.Yeast" rate="0.5/s" />
</module>
```

### Objects

Object definition is same as for `object` in `simulation` just some additional parameters are required.

##### Parameters:

| Name           | Type               | Default                        | Description                                   |
| -------------- | ------------------ | ------------------------------ | --------------------------------------------- |
| `rate`         | `unit[/s]`         | -                              | Rate of object spawning.                      |
| `position-min` | `vector[unit[m]]`  | Left side of simulation scene. | Minimum position where object can be spawned. |
| `position-max` | `vector[unit[m]]`  | Left side of simulation scene. | Maximum position where object can be spawned. |
| `active`       | `array[range[it]]` | -                              | List of ranges when is generator active.      |

> Changing simulation time step affects spawning probability.