---
title: Javascript debugging, Firefox and Drag and Drop
author: denish
layout: post
permalink: /2007/05/javascript-debugging-firefox-and-drag-and-drop/
categories:
  - Blog Entries
  - JavaScript
  - Web development
tags:
  - Debugging
  - 'Drag &amp; Drop'
  - Firefox
  - Javascript
---
In one of my current projects I need to have a table of data that the user can reorder. A quick search of the internet doesn&#8217;t turn up any javascript howtos or frameworks that allow dragging and dropping of table rows, so I had to role my own. I&#8217;ve written an article about how to do it, [here][1].<!--more-->

I use Firefox for initial testing as it gives much better error messages than IE. My Firefox development experience though has been greatly improved since my friend, Netta, pointed me at [Firebug][2]. It&#8217;s totally brilliant, providing a debugger, DOM inspector and nice access to the Javascript console, as well as CSS and HTML viewers and tweakers. I&#8217;m afraid it&#8217;s replaced the Venkman debugger I used to use (and recommended on this blog a few months ago).

Debugging in Internet Explorer still seems years behind and I haven&#8217;t found anything equivalent. I&#8217;ve already recommended the developer toolbar and the latest version of that is even better for viewing the DOM and CSS properties, but for Javascript there doesn&#8217;t seem to be anything apart from the Microsoft Script Debugger which is temperamental to say the least. I also use the debug code published in our [Javascript Debugging][3] article (which should probably be called Javascript Logging really). In fact found a way to display all the properties of a given object which is pretty useful if you&#8217;ve got something and it doesn&#8217;t seem to have the properties you expect. Here it is:

```javascript
function debugShowProperties(object) {
var str = [ '
<table>
  &lt;caption>Properties of object "' + ( object.id || object.toString() ) + '"&lt;/caption>' ];
  var c, i, j = 0;
  
  for( i in object ) {
  var prop = object[i];
  str[++j] = '
  <tr>
    <td align="right">
      '+j+'
    </td>
    <td>
      '+i+'
    </td>
    <td>
      '+prop+'
    </td>
  </tr>
  ';
  }
  str[++j] = "
</table>
";
debug(str.join('n'));
}
```

This creates a table of all the properties of the specified object and outputs it to the debug window using the normal debug call as described in our article.

The code above is based on a snippet I found on the web but I&#8217;m afraid I can&#8217;t find it any more so can&#8217;t put in a link.

 [1]: /articles/table_dnd.php
 [2]: http://www.getfirebug.com/
 [3]: /articles/jsdebug.php
