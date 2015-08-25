
## Reactions
This loader loads simple files that contains reaction rules and some parameters.
Syntax of reaction rules is same as stochastic-reaction plugins.

```
@iterations 500
@dt 1s
null > 0.3 > A;
A > 0.1 > B + C;
```

Lines that begins with `@` symbol are considered as directives that setups simulation.
Remaining lines are parsed by reactions parser and used as program for cell.

Coresponding XML file looks like:
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation dt="1s" iterations="500">
    <program name="__local__" language="stochastic-reactions-intercellular">
<![CDATA[
        null > 0.3 > A;
        A > 0.1 > B + C;
]]>
    </program>
    <object class="cell.Yeast" programs="__local__" />
</simulation>
```

List of available directives:

| Directive    | Parameters             | Description                           | Example           |
| ------------ | ---------------------- | ------------------------------------- | ----------------- |
| `iterations` | `int`                  | Number of simulation iterations.      | `@iterations 100` |
| `dt`         | `time`                 | Simulation iteration step.            | `@dt 10s`         | 
| `molecule`   | `string`, `int`        | Initial amount of specified molecule. | `@molecule A 500` |
| `parameter`  | `string`, `expression` | Set simulation parameter that can be used in reaction rules. | `@parameter kA 0.3` |
