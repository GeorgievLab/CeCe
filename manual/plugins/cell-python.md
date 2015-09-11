
## Cell Python

Adds access to `cell` plugin objects from Python.

### Objects

| Name            | Parent             | Description        |
| --------------- | ------------------ | ------------------ |
| `cell.BaseCell` | `simulator.Object` | Base cell object.  |
| `cell.Yeast`    | `cell.BaseCell`    | Yeast cell.        |

#### Object `cell.BaseCell`

Base class for all cell objects.

##### Properties:

| Name         | Type    | Description       |
| ------------ | ------- | ----------------- |
| `volume`     | `float` | Cell volume       |
| `growthRate` | `float` | Cell growth rate. |

##### Methods:

| Name            | Return  | Arguments | Description       |
| --------------- | ------- | --------- | ----------------- |
| `moleculeCount` | `float` | `string`  | Return amount of required molecule stored in cell. |
| `kill`          | -       | -         | Kills the cell.   |

#### Object `cell.Yeast`

Nothing special.