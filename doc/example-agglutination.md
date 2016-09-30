---
layout: article
permalink: /documentation/examples/agglutination/
title: "Agglutination example"
share: false
categories:
    - documentation
    - example
---

[Source simulation](https://github.com/GeorgievLab/CeCe-examples/blob/master/Agglutination.cece)

This example shows how different ratio of 2 cell populations with binding to each other affects size of created clumps.
Simulation generates default 2000 cells where `1 - ratio` of them are type 1 (red) and `ratio` are type 2 (green).

Ratio and cell number can be changed by passing parameters from CLI application:

```
$ ./cece-cli -p ratio 0.3 -p count 3000 Agglutination.cece
```

Following images shows result for 2000 cells and ratio: 0.1, 0.2 and 0.5.

[![Agglutination ratio 0.1]({{ site.url }}/images/doc/Agglutination_0.1_small.png "Agglutination ratio 0.1")]({{ site.url }}/images/doc/Agglutination_0.1.png)
[![Agglutination ratio 0.2]({{ site.url }}/images/doc/Agglutination_0.2_small.png "Agglutination ratio 0.2")]({{ site.url }}/images/doc/Agglutination_0.2.png)
[![Agglutination ratio 0.5]({{ site.url }}/images/doc/Agglutination_0.5_small.png "Agglutination ratio 0.5")]({{ site.url }}/images/doc/Agglutination_0.5.png)
