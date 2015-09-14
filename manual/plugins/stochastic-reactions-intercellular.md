
## Stochastic reactions - Intercellular

Allows to specify reactions program that happens in surrounding environment. Plugin is an extension of intracellular reactions.

Extension's functionality lies in keyword `env` which is abbreviation for environment, which handles absorbing the molecules inside the cell or producing them outside.
Please note that `env` must be alone on its side of reaction rule.

> Plugin requires properly set `diffusion` module with signals that have same name as molecules released into environment.

This plugin extends basic Intracellular reactions functionality with two types of reactions, expression and absorption.

Expression of molecule `A` is either represented using:

```
A > 1 > env;
```

or using:

```
null > 1 > env_A;
```

Please note that those two representations are not exactly the same although they lead to the same result.
In first example there must be molecule `A` present in the cell, after executing the reaction the molecule gets substracted and added to the environment. Therefore this reaction represents transportation of molecule outside the cell.
In second reaction, the molecule is added and released directly to the environment in single step.

Representation of absorption:

```
env > 1 > A;
```

This reaction follows the same concept. Subtracts molecule from environment and adds inside the cell.
However, you may not want the molecules to get absorbed, you may only want to detect if those molecules are outside.

```
if env_A: B > 1 > C;
```

Extended conditional reactions are the key.
Usage is completely the same as you know from standard intracellular reactions, just add the `env_` prefix before molecule name.
The threshold functionality is of course kept too.

```xml
<parameter name="T1" value="20uM" />
```

```
if env_A > T1: B > 1 > C;
```

##### Example:

This example is production of `A` molecules and releasing them with some rate into diffusion.

```xml
<program name="expression-of-A" language="stochastic-reactions-intercellular"><![CDATA[
  null > 50 > env_A;
]]></program>
```
