
## Stochastic reactions - Intracellular

Allows to specify reactions rules that are stochasticaly being executed inside the cell.

They syntax is similiar to one in NFSim.

This reaction changes molucule `A` to molucule `B` with rate 1.
In other words, when this reaction occurs, one molecule A gets substracted and one molecule `B` gets added.

```
A > 1 > B;
```

This reaction changes one molucule `A` into molucules `B` and `C`.

```
A > 1 > B + C;
```

If you need reaction to happen only when some other molecule is present, take a look at following reaction.
This reaction subtracts `A` and `B`, and adds `C + B`. Therefore, this reaction changes `A` to `C` only when `B` is present.
Please note you can't use this concept with environmental reactions.

```
A + B > 3 > C + B;
```

This reaction creates complex `C` from 2 molucules of `A`.
That means that at least two As are required for this reaction to occur.

```
A + A > 2 > C;
```
                    
This reaction uses keyword `null` and represents expression of `A`.
No molecule gets substracted and one molucule of `A` gets added.

```
null > 2 > A;
```

Similiarly, this reaction repressents degradation of `A`.

```
A > 5 > null;
```

Those two reactions can be easily rewritten like this using reversible reaction syntax.
First rate is rate of reaction going back, and second rate is for reaction going forward.

```
A < 5, 2 > null;
null < 2, 5 > A;
```

Another keyword which helps user to make his reaction rules more understandable is `if`, `and` and `or`.

```
if not C and D and E:  A + B > 1 > C;
```

This reaction merges `A` and `B` into `C`, but this reaction can occur only when there is no molucule of `C` present and simultaneously there must be `D` and `E` present.
You can combine as many logic combinations as you can.
Please note that `and` is prior to `or`, this means that following reaction only occur when there is either `B` and `C` together or `A` present in the cell.

```
if A or B and C: D > 1 > E;
```

##### Example:

Defines reactions that create GFP molecule with some degradation.

```
<program name="make-gfp" language="stochastic-reactions-intracellular"><![CDATA[
  null > 1.505149978 > GFP;
  GRP > 0.001 > null;
]]></program>
```
