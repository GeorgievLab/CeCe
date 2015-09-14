% The CeCe simulator tutorial
% Georgiev Lab (c) 2015

# Basics

The simulator uses simulation files written in *XML* with some structure that is understandable for the simulator. Structure is pretty understandable so good knowledge of XML files is not required. Let's go to do some coding.

Following code defines an empty simulation with world size of 200um x 200um.

```xml
<simulation world-size="200um 200um">
</simulation>
```

As you can see, the simulation is defined by XML file with specific structure. There always must be `simulation` element as root element.

Let's add some objects...

```xml
<simulation world-size="200um 200um">
    <object class="cell.Yeast" position="10um 0" />
    <object class="cell.Yeast" position="-10um 0" />
</simulation>
```

Now we have defined 2 yeasts cells at position (-10, 0) and (10, 0). The attribute `class` defines a class of the object. Value is divided into two parts where the first one is a plugin name and the second one is class name from that plugin. Plugins are a way how the simulator is extended.

Try to run the simulation now. You can see how the yeast cells are growing and budding...

If you think this is boring (and it is), try to modify cells to produce some molecules.

```xml
<simulation world-size="200um 200um">
    <program name="produce-gfp" language="stochastic-reactions-intracellular">
null > 1 > GFP;
    </program>
    <program name="produce-rfp" language="stochastic-reactions-intracellular">
null > 1 > RFP;
    </program>
    <object class="cell.Yeast" position="10um 0" programs="produce-gfp" />
    <object class="cell.Yeast" position="-10um 0" programs="produce-rfp" />
</simulation>
```

The yeast cells are now producing GFP and RFP molecules in each simulation step. After some time they should change their color depending on how many molecules are inside them. The `program` element defines new program with given name under that the program is available for objects. In this case the programs are defined by language from `stochastic-reactions-intracellular` plugin that allows to specify stochastic reactions inside cells. Both of these programs produce molecules constitutively (for more information see the plugin's manual). Programs are unused until are bind to some object. One program can be used by multiple objects and object can use multiple programs. In case you want to object to use more programs, just separate programs names by space like this.

```xml
<object class="cell.Yeast" position="10um 0" programs="produce-gfp produce-rfp" />
```

You maybe think why the cells changes their color. This is because they contains fluorescent proteins. There are some predefined molecule names that changes cell color (GFP, RFP, YFP, CFP and BFP - must be uppercase).

As next step, we use some modules that changes global behaviour of our simulation. Our goal is change object to produce molecules into environment.

```xml
<simulation world-size="200um 200um">
    ...
    <module name="diffusion" grid="200 200">
        <signal name="GFP" color="green" />
        <signal name="RFP" color="red" />
    </module>
</simulation>
```

This code adds `diffusion` module from `diffusion` plugin. It requires to specify grid size and then list of signals that are allowed in diffusion. Each signal must have defined name under that is accessible. You also can specify signal color to distinguish between signals. There are more parameters that can be changed for each signal, see for more in `diffusion` plugin manual.

Let's change the programs to produce molecules in environment.

```xml
    ...
    <program name="produce-gfp" language="stochastic-reactions-intercellular">
null > 1 > env_GFP;
    </program>
    <program name="produce-rfp" language="stochastic-reactions-intercellular">
null > 1 > env_RFP;
    </program>
    ...
```

There is important change that you might not noticed. Program language has changed from `stochastic-reactions-intracellular` to `stochastic-reactions-intercellular`. This slightly different language allows to communicate with diffusion module. Molecule names with `env_` prefix are taken/given from/to diffusion module. The important thing to note is the reactions use number of molecules and diffusion use concentration. Conversion is done in background but in case you use environment molecule in conditions it needs to keep this on mind.

This gives you some basic ideas how to define own simulation files. For more information look into examples and look into manual.
