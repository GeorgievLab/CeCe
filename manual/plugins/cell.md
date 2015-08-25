
## Cell
Plugin offers things usefull to working with cells.

### Objects

Objects offered by `cell` plugin:

| Name            | Description         |
| --------------- | ------------------- |
| `cell.Cell`     | Common cell object. |
| `cell.Yeast`    | Yeast cell.         |

All object share common (unavailable) ancestor that have following properties:

| Property      | Units           | Description       | Example  |
| ------------- | --------------- | ----------------- | -------- |
| `volume`      | m<sup>3</sup>   | Cell volume       | `300um3` |
| `growth-rate` | m<sup>3</sup>/s | Cell growth rate. | `3um3/s` |

Cell can have set initial amount of molecule at the beginning.

```xml
<object class="cell.Yeast">
  <molecule name="GFP" amount="500" />
</object>
```

### Programs
#### `store-molecules`
Program that stores amount of all molecules in current iteration to `molecules` data table. It stores object identifier to distinguish between multiple cells.

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `id`        - Object identifier.
* `...`       - Column given by molecule name and value is amount of that molecule.

## Cell Python
Adds access to `cell` plugin objects from Python.

### Objects

| Name            | Parent             | Description        |
| --------------- | ------------------ | ------------------ |
| `cell.BaseCell` | `simulator.Object` | Base cell object.  |
| `cell.Yeast`    | `cell.BaseCell`    | Yeast cell.        |

#### Object `cell.BaseCell`

##### Properties:

| Name         | Type    | Description       |
| ------------ | ------- | ----------------- |
| `volume`     | `float` | Cell volume       |
| `growthRate` | `float` | Cell growth rate. |

##### Methods:

| Name            | Return  | Arguments | Description       |
| --------------- | ------- | --------- | ----------------- |
| `moleculeCount` | `float` | `string`  | Return amount of required molecule stored in cell. |
