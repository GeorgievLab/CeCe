---
layout: article
permalink: /documentation/sample-simulation/
title: "Sample simulation"
share: false
categories:
    - documentation
---

Following code shows sample CeCe simulation with brief explanation of some used features.

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!--
  Sample simulation definition.

  world-size: physical size of the simulation world.
  dt: physical time step.
  iterations [0]: Total number of simulation iterations. Value 0 or value omitting means unlimited iterations.
  visualized [true]: If simulation is visualized.
  background [white]: Background color. Can be in CSS form: #FFFFFF.
-->
<simulation world-size="300um 300um" dt="1s" iterations="10000" visualized="true" background="white">

    <!--
      Imported plugins. Importing a specific plugin allows to simulation use plugin specific
      features (object types, modules, ...).

      Call to get a list of available plugins.
      $ ./bin/cece-cli --plugins
    -->
    <plugin name="cell" />                  <!-- adds cell.Yeast -->
    <plugin name="obstacle" />              <!-- adds obstacle.Rectangle -->
    <plugin name="python" />                <!-- adds python program support -->
    <plugin name="cell-python" />           <!-- adds python support for cell plugin -->
    <plugin name="stochastic-reactions" />  <!-- adds stochastic-reactions program support -->
    <plugin name="diffusion" />             <!-- adds diffusion support -->

    <!--
      Create a diffusion module.
    -->
    <module name="diffusion" grid="200 200">
      <signal name="Protein2" diffusion-rate="50um2/s" degradation-rate="0.01/s" color="blue" saturation="13nM" />
    </module>

    <!--
      Object program definition. Programs can be bound to simulation objects and are executed each
      iteration for reach object they are bound to.

      name: Program name under which will be available.
      language/type: Language or type which can understand program definition.
    -->
    <program name="program1" language="python">
<![CDATA[
import core
def __call__(object, simulation, dt):
    # Print GFP & RFP amount
    print("GFP: " + str(object.moleculeCount("GFP")))
    print("RFP: " + str(object.moleculeCount("RFP")))
    print("Pr1: " + str(object.moleculeCount("Protein1")))
]]>
    </program>

    <!--
      Stochastic reactions program.
    -->
    <program name="program2" language="stochastic-reactions">
<![CDATA[
        # GFP degradation
        GFP > 0.2 > null;
        # RFP production
        null > 0.7 > RFP;
        # Protein2 production into environment in case of required amount of Protein1
        if Protein1 > 500: Protein1 > 0.3 > env Protein2;
]]>
    </program>

    <!--
      Static definition of simulation object. Object can be obstacle or cell.

      class: Object class.
      position: Initial position in the simulation world. [0 0] is in the middle of the world.
      programs: space separated list of object programs.
    -->
    <object class="cell.Yeast" position="0um 10um" programs="program1 program2">
        <!--
          Object class 'cell.Yeast' allows you to specify initial amount of molecules.

          Molecules GFP, RFP & YFP is used for cell color in visualization.
        -->
        <molecule name="GFP" amount="100000" />
        <molecule name="RFP" amount="1000" />
        <molecule name="Protein1" amount="500" />
    </object>

    <!--
      Create a black rectangle obstacle at position [0 50um] with size [100um 10um].
    -->
    <object class="obstacle.Rectangle" position="0 50um" size="100um 10um" show="true" color="black" />
</simulation>
```