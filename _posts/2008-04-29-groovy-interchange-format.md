---
title: Groovy Interchange Format?
author: denish
layout: post
permalink: /2008/04/groovy-interchange-format/
categories:
  - Groovy
  - Java
  - Web development
tags:
  - Graphs
  - Groovy
  - Java
  - Web
---
Is Groovy a good way to pass complex data from one server to another (instead of JSON or XML)?  
<!--more-->

  
I&#8217;m in the process of developing some servlet-based graphs and need to pass complex data to the graph so that it has enough information to draw a scatter chart and overlay this with an image map so that the user can hover over and click on the individual data points.

The way I want it to work is to have a stand-alone image producing servlet and pass it a data URI in the request which it can open and get the data it needs.

There are a number of different ways I could use to encapsulate this data: 

  * I could pass Java objects of some class, but then I would have to pass some sort of query in the URL so that the server can get the right objects to display. It would be difficult to decouple the graphics from the data server using this approach. And it certainly wouldn&#8217;t work from one server to another.
  * I could use XML and pass the servlet could parse the XML and have all the data that it needs. This would seem to be the &#8220;standard&#8221; approach. But parsing XML isn&#8217;t cost free both in terms of time or programming effort, so though it&#8217;s a well-understood problem, I&#8217;m not jumping to choose it.
  * I could use JSON. This seems quite a neat approach and I&#8217;ve looked at other Graph generating solutions (for example [Open Flash Chart][1]). There&#8217;s the easy to use [JSON Tools][2] that would make this OK. But you still have to walk through the JSON Tools objects and extract the data.
  * Or, since I&#8217;m already using Groovy on this project, I could use Groovy.

The advantage of using Groovy is that, like JSON, it&#8217;s very easy and pretty compact to write. The sort of data I&#8217;m looking at is:

```groovy
[
    points: [
        [10, 10, "Visit 1", "javascript:alert('Visit 1')"], 
        [10, 40, "Visit 2"], 
        [40, 10, "Visit 3"], [40, 40, "Visit 4"]
    ],
    xLabel: 'Waiting Time',
    yLabel: 'Cost per visit'
]
```

So effectively, I&#8217;m creating a simple Map (for the points I&#8217;m using a list because the order may be significant).

It&#8217;s extensible because I can add new keys and values (though the same applies to XML or JSON). But the best bit is that in order to parse it, all you have to do is:

```groovy
GroovyShell shell = new GroovyShell();
    Map data = (Map) shell.evaluate(groovyData);

    String yLabel = (String) data.get("yLabel");
    ....
```
Obviously, this is a simplification because I need to check that the Groovy is well formed, parsable and in the format that we expect, but it isn&#8217;t any more difficult than it would be in XML or JSON.

Any thoughts or comments on this approach would be welcome!

 [1]: http://teethgrinder.co.uk/open-flash-chart/
 [2]: http://jsontools.berlios.de/
