## Streamlines Python

Python language binding for `streamlines` plugin.

##### Example:

This module generates in every iteration some diffusion signal inside defined circle.

```xml
<module name="python:generate"><![CDATA[
layouts = [
    ["barrier", "outlet", "barrier", "inlet"],
    ["barrier", "inlet", "barrier", "outlet"]
]

mode = 1

def update(dt, simulation):
    global mode
    module = simulation.useModule("streamlines")

    if (simulation.iteration % 200 == 0):
        module.setLayout(layouts[mode % 2])
        module.initBarriers(simulation)
        mode = mode + 1
]]></module>
```

### Constants:

| Name     | Type   | Description  |
| -------- | ------ | ------------ |
| `LEFT`   | `uint` | Left side.   |
| `RIGHT`  | `uint` | Right side.  |
| `TOP`    | `uint` | Top side.    |
| `BOTTOM` | `uint` | Bottom side. |

### Class `streamlines.Module`

Streamlines module wrapping class.

##### Methods:

| Name               | Return   | Arguments                              | Description                                    |
| ------------------ | -------- | -------------------------------------- | ---------------------------------------------- |
| `setLayout`        | `int`    | `string`, `string`, `string`, `string` | Set streamlines layout. Top, right, bottom, left. Possible values `barrier`, `inlet`, `outlet`, `none`. |
| `initBarriers`     | `float`  | -                                      | Initialize barriers after setLayout call.      |
| `setInletVelocity` | -        | `int`, `float`                         | Change inlet velocity for specific scene side (see constants). |
