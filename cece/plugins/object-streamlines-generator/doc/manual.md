
# Object streamlines generator plugin

Alternative plugin to 'object-generator' which can use information from 'streamlines' plugin
like inlet flow rate and boundary positions.

## Example:

This example create module that generates different Yeast cells in each iteration with some probability.

```xml
<plugin name="object-streamlines-generator" />
<plugin name="streamlines" />
<module name="streamlines>
  <boundary type="inlet" name="inlet1" inlet-flow-rate="10ml/s" />
</module>
<module name="object-generator">
  <object class="cell.Yeast" concentration="500/ml" boundary="inlet1" />
</module>
```
