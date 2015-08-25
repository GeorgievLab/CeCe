
## Diffusion Python

Python language binding for `diffusion` plugin.

##### Example:

```xml
<module name="python:generate"><![CDATA[
import math

def update(dt, simulation):
    diffusion = simulation.useModule("diffusion")
    size = diffusion.gridSize
    radius = size.x / 20
    signalAmount = 1

    for x in range(-radius, radius + 1):
        for y in range(-radius, radius + 1):
            if (math.sqrt(x * x + y * y) <= radius):
                diffusion.setSignal(0, size.x / 2 + x, size.y / 2 + y, signalAmount)
]]></module>
```

### Class `diffusion.Module`

Diffusion module wrapping class.

##### Properties:

| Name          | Type           | Description                  |
| ------------- | -------------- | ---------------------------- |
| `gridSize`    | `vector[uint]` | Grid size.                   |
| `signalCount` | `uint`         | Number of different signals. |

##### Methods:

| Name        | Return  | Arguments                       | Description                                               |
| ------------| ------- | ------------------------------- | --------------------------------------------------------- |
| `getSignal` | `float` | `string`, `int`, `int`          | Return signal value of given signal at given coordinates. |
| `setSignal` | -       | `string`, `int`, `int`, `float` | Change signal value of given signal at given coordinates. |

## Diffusion Streamlines

Without this plugin/module the streamlines does not affect diffusion. Just simply add:

```xml
<module name="diffusion-streamlines" />
```

> It doesn't require from diffusion and streamlines module to have same grid sizes.
