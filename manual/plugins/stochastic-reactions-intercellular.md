
## Stochastic reactions - Intercellular

Allows to specify reactions program that happens inside cell and surrounding environment. Plugin is just an extension of intracellular reactions.

The extension lies in keywork "env" - environment, which handles absorbing the molucules inside the cell or prodicing them outside.
Please note that env must be alone on its side of reaction rule.

> Plugin requires properly set `diffusion` module with signals that have same name as molecules released into environment.

##### Example:

This example is production of A molecules and releasing them with some rate into diffusion.

```
<program name="make-gfp" language="stochastic-reactions-intercellular"><![CDATA[
  null < 0.001, 50 > A;
  A > 0.9 > env;
]]></program>
```
