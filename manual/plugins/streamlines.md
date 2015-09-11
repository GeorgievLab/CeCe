## Streamlines

Module that simulate streamlines in whole scene. It using Lattice Boltzman method to calculate velocities in simulation grid. Simulation can handle static and even dynamic obstacles. Dynamic obstacles are moving objects in scene. Dynamic obstacles (i.e. objects) are affected by computed velocities.

There are some limitations that come from Lattice Boltzman method. Maximum velocity is limited by grid size, time step and scene/world size. If the velocity is higher than this limit, it's changed to the maximum otherwise the streamlines simulation crashes.

##### Example:

```xml
<module name="streamlines" grid="200" inlet-velocity="300um/s" kinematic-viscosity="0.658mm2/s" />
```

##### Parameters:

| Name                  | Type           | Default     | Description |
| --------------------- | -------------- | ----------- | ----------- |
| `grid`                | `vector[uint]` | -           | Simulation grid size. |
| `layout`              | 4x `string`    | `barrier outlet barrier inlet` | Defines scene layout. Four values define layout in order: **top** **right** **bottom** **left**. Possible values are: `none`, `barrier`, `inlet`, `outlet`. |
| `inlet-velocity`      | `unit[m/s]`    | `0`         | Velocity for all inlets. |
| `inlet-velocities`    | 4x `unit[m/s]` | `0 0 0 0`   | Specific inlet velocities for layout. |
| `kinematic-viscosity` | `unit[m2/s]`   | `0.658mm/s` | Fluid kinematic viscosity. |

### Additional modules

#### `store-state`

Module that stores velicities from grid into `streamlines` data table.

##### Example:

```xml
<module name="streamlines.store-state" />
```

> This module generates a huge amount of data so use with caution.

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `x`         - Grid X coordinate.
* `y`         - Grid Y coordinate.
* `velX`      - Velocity in specified grid cell, X coordinate.
* `velY`      - Velocity in specified grid cell, Y coordinate.

### Additional programs

#### `store-object-state`

Module that stores object position and velicities into `object-state` data table.

##### Example:

```xml
<object ... programs="streamlines.store-object-state" />
```

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `id`        - Object identifier.
* `x`         - Object X world coordinate.
* `y`         - Object Y world coordinate.
* `velX`      - Object velocity, X coordinate.
* `velY`      - Object velocity, Y coordinate.
