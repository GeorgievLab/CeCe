
## Stochastic reactions - Intracellular

Allows to specify reactions rules that are stochastically being executed inside the cell.

> The syntax is similar to one in NFSim.

This reaction changes molecule `A` to molecule `B` with rate 1.
In other words, when this reaction occurs, one molecule `A` gets substracted and one molecule `B` gets added.

```
A > 1 > B;
```

This reaction changes one molecule `A` into molecules `B` and `C`.

```
A > 1 > B + C;
```

If you need reaction to happen only when some other molecule is present, take a look at following reaction.
This reaction subtracts `A` and `B`, and adds `C + B`. Therefore, this reaction changes `A` to `C` only when `B` is present.
Please note you can't use this concept with environmental reactions.

```
A + B > 3 > C + B;
```

This reaction creates complex `C` from 2 molecules of `A`.
That means that at least two As are required for this reaction to occur.

```
A + A > 2 > C;
```

This reaction uses keyword `null` and represents expression of `A`.
No molecule gets substracted and one molecule of `A` gets added.

```
null > 2 > A;
```

Similarly, this reaction represents degradation of `A`.

```
A > 5 > null;
```

The two reactions above can be easily rewritten like this using reversible reaction syntax.
First rate is rate of reaction going back, and second rate is for reaction going forward.

```
A < 5, 2 > null;
null < 2, 5 > A;
```

Another keywords which help user to make his reaction rules more understandable are `if`, `and` and `or`.

```
if not C and D and E:  A + B > 1 > C;
```

This reaction merges `A` and `B` into `C`, but this reaction can occur only when there is no molecule of `C` present and simultaneously there must be `D` and `E` present.
You can combine as many logic combinations as you can.
Please note that `and` is prior to `or`, this means that following reaction only occur when there is either `A` or `B` and `C` together present in the cell.

```
if A or B and C: D > 1 > E;
```

We want to make your reactions easily tunable, so we included the principle of threshold.
The following reaction get executed only when there is 500 molecules of `B` present in the cell.

```
If B > 500: A > 1 > C;
```

##### Example:

Defines reactions that create molecules `GFP` and `A` with some degradation.

```xml
<parameter name="K1" value="20" />
<program name="make" language="stochastic-reactions-intracellular"><![CDATA[
  if A > K1: null > 1.505149978 > GFP;
  GFP > 0.001 > null;
  null < 0.5, 0.8 > A;
]]></program>
```
