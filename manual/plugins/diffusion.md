
## Diffusion
  
The main module adds support for global diffusion in simulation. Diffusion can handle any number of separate signals. They cannot affect each other.

##### Example:

Enables diffusion with two signals named *GFP* and *RFP* with diffusion and degradation rates. Those signals have specified colors how they are rendered.

```xml
<module name="diffusion" grid="100">
  <signal name="GFP" diffusion-rate="30um2/s" degradation-rate="0.1/s" color="green" />
  <signal name="RFP" diffusion-rate="5um2/s" color="red" saturation="1uM" />
</module>
```

##### Parameters:

| Name         | Type           | Default        | Description |
| ------------ | -------------- | -------------- | ----------- |
| `grid`       | `vector[uint]` | -              | Diffusion grid size. |
| `background` | `color`        | `transparent`  | Background color. |

### Signals

You can specify any number of different signals, there is no limitation. Each signal is specified separately.

##### Parameters:

| Name               | Type           | Default        | Description |
| ------------------ | -------------- | -------------- | ----------- |
| `name`             | `string`       | -              | Signal name. |
| `diffusion-rate`   | `unit[m2/s]`   | -              | Diffusion rate. |
| `degradation-rate` | `unit[/s]`     | `0/s`          | Degradation rate. |
| `color`            | `color`        | Predefined     | Signal color when the module is rendered. |
| `saturation`       | `unit[mol/m3]` | `1umol/um3`    | Defines concentration when signal color is 100%. |

### Additional modules

#### `store-state`

Module that stores values from signal grid of all signals into `diffusion` data table.

##### Example:

Store diffusion data for all iterations.

```xml
<module name="diffusion.store-state" />
```

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `x`         - Grid X coordinate.
* `y`         - Grid Y coordinate.
* `...`       - Column given by molecule name and value is amount of that molecule in grid cell.
