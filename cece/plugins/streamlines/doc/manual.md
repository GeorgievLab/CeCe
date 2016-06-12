# Streamlines Plugin

Module that simulate streamlines in whole scene. It using Lattice Boltzman method to calculate velocities in simulation grid. Dynamic obstacles (i.e. objects) are affected by computed velocities.

There are some limitations that come from Lattice Boltzman method. Maximum velocity is limited by grid size, time step and scene/world size. If the velocity is higher than this limit, it's changed to the maximum otherwise the streamlines simulation crashes.

## Example:

```xml
<module name="streamlines" inlet-velocity="300um/s" kinematic-viscosity="0.658mm2/s" />
```

## Streamlines module

### Parameters:

| Name                  | Type           | Default     | Description |
| --------------------- | -------------- | ----------- | ----------- |
| `layout`              | 4x `string`    | `barrier outlet barrier inlet` | Defines scene layout. Four values define layout in order: **top** **right** **bottom** **left**. Possible values are: `none`, `barrier`, `inlet`, `outlet`. |
| `inlet-velocity`      | `unit[m/s]`    | `0`         | Velocity for all inlets. |
| `inlet-velocities`    | 4x `unit[m/s]` | `0 0 0 0`   | Specific inlet velocities for layout. |
| `kinematic-viscosity` | `unit[m2/s]`   | `0.658mm/s` | Fluid kinematic viscosity. |
| `init-iterations`     | `int`          | `0`         | Number of initial iteration that setup the streamlines. |
| `init-file`           | `filename`     | -           | Path to precomputed streamlines file. Expression '%temp%' can be used to substitute OS's temporary directory. |
| `char-length`         | `unit[m]`      | `1um`       | Characteristic length. |
| `number-nodes`        | `int`          | `1`         | Number of simulation nodes per characteristic length. |
| `tau`                 | `float`        | -           | LBM Relaxation parameter. |
| `layer-magnitude`     | `name`         | -           | Name of visualization layer used for velocity rendering. |
| `layer-density`       | `name`         | -           | Name of visualization layer used for density rendering. |
| `layer-dynamics`      | `name`         | -           | Name of visualization layer used for fluid nodes rendering. |