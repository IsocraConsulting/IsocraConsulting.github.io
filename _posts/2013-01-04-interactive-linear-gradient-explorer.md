---
title: Interactive Linear Gradient Explorer
author: denish
layout: post
permalink: /2013/01/interactive-linear-gradient-explorer/
categories:
  - Blog Entries
  - Flash
  - Flex
tags:
  - Flash
  - Flex
  - Spark
---
Linear Gradients in Flex are very powerful, but it&#8217;s not necessarily easy to know what values to use. This interactive tool allows you to experiment with adding and configuring as many Gradient Entries as you need so that you can see the effect immediately.<!--more-->

Once you have the effect you want, you can copy the MXML source code below to use in your Flex projects

[kml_flashembed movie=&#8221;/flash/LinearGradientExplorer.swf&#8221; /]

You need at least two linearGradients. If you don&#8217;t specify a ratio for either of them then the effect is a smooth transition from the first colour to the second right through the shape you are trying to fill. This is the same as having a ratio of 0 for the first linearGradient and 1 for the second.

Specifying a ratio tells Flex to start the transition at that point. So setting the ratio of the first linearGradient to 0.5 means that the transtion starts 50% of the way through the shape and continues through to the end (as shown below).

```html
<s:LinearGradient rotation="90">
    <s:GradientEntry color="0x000000" ratio="0.5"/>
    <s:GradientEntry color="0xffffff" ratio="1"/>
</s:LinearGradient>
```

Each extra linearGradient works in the same way, it acts as the destination colour and boundary for the previous transition and the starting point for the next.

In the following example you can see that the shape stays yellow for the first 30%, then transitions from yellow to red by the time it reaches 65% and then transitions from red to blue.

```html
<s:LinearGradient rotation="90">
    <s:GradientEntry color="0xffff00" ratio="0.3"/>
    <s:GradientEntry color="0xff0000" ratio="0.65"/>
    <s:GradientEntry color="0x0000ff" ratio="1"/>
</s:LinearGradient>
```
