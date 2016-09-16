---
layout: article
permalink: /documentation/writing-a-simulation/
title: "Writing a simulation"
share: false
categories:
    - documentation
---

Writing a custom simulation requires some basic knowledge of XML language. Language is used to describe initial static simulation state and some special parts can describe dynamic simulation behaviour.

The simplest simulation that can be run is:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s" />
```

It doesn't contain or do anything. It defines a simulation with 300um x 300um simulation world with simulation step which takes 1 second.

You can add an object to the simulation but first you need to import a plugin with defines your object because core CeCe doesn't have support for anything usable. Because we want to add a cell object we need to import `cell` plugin which contains support for basic cell objects.

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s">
   <plugin name="cell" />
</simulation>
```

Now we can add a cell object.

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s">
   <plugin name="cell" />
   <object class="cell.Yeast" />
</simulation>
```

If your run this simulation, you can see a single cell in the middle of the simulation area. Object position can be specified by `position` attribute (the first value is X-coordinate and second one is Y-coordinate).

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s">
   <plugin name="cell" />
   <object class="cell.Yeast" position="-10um 0" />
   <object class="cell.Yeast" position="10um 0" />
</simulation>
```

Now we have 2 yeast cells on different position which do nothing. Let's give them some action. Each simulation object can have programs which operate separately with object data. Program can be written in different languages you just need support from plugin.

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s">
   <plugin name="cell" />
   <plugin name="stochastic-reactions" />

   <program name="make-gfp" language="stochastic-reactions">
<![CDATA[
    null > 0.3/s > GFP;
]]>
   </program>

   <program name="make-rfp" language="stochastic-reactions">
<![CDATA[
    null > 0.3/s > RFP;
]]>
   </program>

   <object class="cell.Yeast" position="-10um 0" programs="make-gfp" />
   <object class="cell.Yeast" position="10um 0"  programs="make-rfp" />
</simulation>
```

You can see we defined two programs `make-gfp` and `make-rfp` which are written in `stochastic-reactions` language (plugin with support must be imported). We don't explain `stochastic-reactions` language for that you need to look into plugin's documentation but they just produce *GFP* or *RFP* molecules within cell object. They need to be assigned to specific cell object by `programs` attribute.

`cell.Yeast` object type has speciality when they convert fluorescence proteins within into visible color so you can see how they slowly changing their color from black to green or red.

Let's change the simulation a little bit with adding diffusion support.

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<simulation world-size="300um 300um" dt="1s">
   <plugin name="cell" />
   <plugin name="stochastic-reactions" />
   <plugin name="diffusion" />

   <program name="make-gfp" language="stochastic-reactions">
<![CDATA[
    null > 0.3/s > GFP;
    null > 0.8/s > env Protein1;
]]>
   </program>

   <program name="make-rfp" language="stochastic-reactions">
<![CDATA[
    null > 0.3/s > RFP;
    null > 0.8/s > env Protein2;
]]>
   </program>

    <module name="diffusion" grid="300 300">
        <signal name="Protein1" diffusion-rate="50um2/s" degradation-rate="0.01/s" color="blue" saturation="13nM" />
        <signal name="Protein2" diffusion-rate="50um2/s" degradation-rate="0.01/s" color="yellow" saturation="13nM" />
    </module>

   <object class="cell.Yeast" position="-10um 0" programs="make-gfp" />
   <object class="cell.Yeast" position="10um 0"  programs="make-rfp" />
</simulation>
```

We added a diffusion module. Module is a global scope program which affects whole scene and not just one object (e.g. diffusion, streamlines). We set diffusion simulation grid to 300x300 and defined supported signals named `Protein1` and `Protein2` and their parameters. Stochastic reactions programs have support for diffusion so we can add reactions which produce signals in environment (diffusion). Names in reactions and diffusion must match.

Running simulation shows you a nice scene with two color changing cells with colored clouds around them.

This is a basic explanation how CeCe simulation can be written, for more details look into [examples](https://github.com/GeorgievLab/CeCe-examples) and [plugins](https://github.com/GeorgievLab/CeCe-plugins) documentation.
