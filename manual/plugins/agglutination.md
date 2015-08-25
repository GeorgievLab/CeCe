
## Agglutination

This plugin enables simulation object sticking. When two simulation object collides they might be bound together and after some time the bound can be removed. This is based on bond definition where you can specify required molecules in both collided cells to create a bond. Specifying association and dissociation constant you indirectly defines probability of bond creation and destruction.

##### Example:

Example defines bonds between cells.

```xml
<module name="agglutination">
  <bond ligand="LM1" receptor="LM3" association-constant="50" disassociation-constant="1" />
  <bond ligand="LM2" receptor="LM4" association-constant="20" disassociation-constant="10" />
  <bond ligand="LM5" receptor="LM6" association-constant="1"  disassociation-constant="0" />
</module>
```

### Bonds

##### Parameters:

| Name                      | Type    | Default | Description                  |
| ------------------------- | ------- | ------- | ---------------------------- |
| `ligand`                  | `name`  | -       | Name of the first molecule.  |
| `receptor`                | `name`  | -       | Name of the second molecule. |
| `association-constant`    | `float` | -       | Bond association constant.   |
| `disassociation-constant` | `float` | -       | Bond dissociation constant.  |
