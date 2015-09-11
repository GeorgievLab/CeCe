
## Cell

Plugin offers things usefull to working with cells.

### Objects

Objects offered by `cell` plugin:

| Name            | Description         |
| --------------- | ------------------- |
| `cell.Cell`     | Common cell object. |
| `cell.Yeast`    | Yeast cell.         |

All object share common (unavailable) ancestor that have following properties:

| Property         | Units                  | Default  | Description                                                   | Example  |
| ---------------- | ---------------------- | -------- | ------------------------------------------------------------- | -------- |
| `volume`         | m<sup>3</sup>          | `100um3` | `Cell volume                                                  | `300um3` |
| `volume-max`     | m<sup>3</sup>          | `100um3` | Maximum cell volume.                                          | `300um3` |
| `growth-rate`    | m<sup>3</sup>/s        | `0`      | Cell growth rate.                                             | `3um3/s` |
| `saturation-gfp` | Molecule/m<sup>3</sup> | `20/um3` | Number of "GFP" molecules required to have full green color.  | `100/um3` |
| `saturation-rfp` | Molecule/m<sup>3</sup> | `20/um3` | Number of "RFP" molecules required to have full red color.    | `100/um3` |
| `saturation-yfp` | Molecule/m<sup>3</sup> | `20/um3` | Number of "YFP" molecules required to have full yellow color. | `100/um3` |
| `saturation-cfp` | Molecule/m<sup>3</sup> | `20/um3` | Number of "CFP" molecules required to have full cyan color.   | `100/um3` |
| `saturation-bfp` | Molecule/m<sup>3</sup> | `20/um3` | Number of "BFP" molecules required to have full blue color.   | `100/um3` |

Cell can have set initial amount of molecule at the beginning.

```xml
<object class="cell.Yeast">
  <molecule name="GFP" amount="500" />
</object>
```

#### Yeast

| Property             | Units         | Default | Description                         | Example  |
| -------------------- | ------------- | ------- | ----------------------------------- | -------- |
| `volume-bud-create`  | m<sup>3</sup> | `42um3` | Yeast volume when a bud is created. | `300um3` |
| `volume-bud-release` | m<sup>3</sup> | `35um3` | Bud volume when is release.         | `300um3` |

### Programs

#### `store-molecules`

Program that stores amount of all molecules in current iteration to `molecules` data table. It stores object identifier to distinguish between multiple cells.

```xml
<object class="cell.Yeast" programs="cell.store-molecules" />
```

##### Stored data:

* `iteration` - Iteration number.
* `totalTime` - Simulation time in seconds.
* `id`        - Object identifier.
* `...`       - Column given by molecule name and value is amount of that molecule.
