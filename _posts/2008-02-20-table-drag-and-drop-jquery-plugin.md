---
title: Table Drag and Drop JQuery plugin
author: denish
layout: post
permalink: /2008/02/table-drag-and-drop-jquery-plugin/
categories:
  - Articles
  - JavaScript
  - jQuery
  - Web development
tags:
  - 'Drag &amp; Drop'
  - Javascript
  - Web
extra_js:
  - https://rawgit.com/isocra/TableDnD/master/js/jquery.tablednd.js
---
I&#8217;ve been using [JQuery][1] for a while now and really agree with its tag line that it&#8217;s the &#8220;The Write Less, Do More, JavaScript Library&#8221;. We&#8217;ve also got this [code for dragging and dropping][2] table rows that has proved very popular, so it seemed natural to combine the two and wrap up the table drag and drop as a JQuery plugin.<!--more-->

  


## Why have another plugin?

Dragging and dropping rows within a table can&#8217;t be handled by general purpose drag and drop utilities for a number of reasons, not least because you need to move the whole row, not just the cell that receives the mouse events. Re-parenting the row also requires specific code. Sadly also, effects like fadeIn and fadeOut don&#8217;t work well with table rows on all browsers, so we have to go for simpler effects.

## What does it do?

This TableDnD plugin allows the user to reorder rows within a table, for example if they represent an ordered list (tasks by priority for example). Individual rows can be marked as non-draggable and/or non-droppable (so other rows can&#8217;t be dropped onto them). Rows can have as many cells as necessary and the cells can contain form elements.

## How do I use it?

  1. Download [Download jQuery][1] (version 1.2 or above), then the [TableDnD plugin][3] (current version 0.7).
  2. Reference both scripts in your HTML page in the normal way.
  3. In true jQuery style, the typical way to initialise the tabes is in the `$(document).ready` function. Use a selector to select your table and then call `tableDnD()`. You can optionally specify a set of properties (described below).

<div class="tableDemo">
  <table id="table-1" cellspacing="0" cellpadding="2">
    <tr id="1">
      <td>
        1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="2">
      <td>
        2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="3">
      <td>
        3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="4">
      <td>
        4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="5">
      <td>
        5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="6">
      <td>
        6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        some text
      </td>
    </tr>
  </table>
</div>

The HTML for the table is very straight forward (no Javascript, pure HTML):

{% highlight html %}<table id="table-1" cellspacing="0" cellpadding="2">
    <tr id="1"><td>1</td><td>One</td><td>some text</td></tr>
    <tr id="2"><td>2</td><td>Two</td><td>some text</td></tr>
    <tr id="3"><td>3</td><td>Three</td><td>some text</td></tr>
    <tr id="4"><td>4</td><td>Four</td><td>some text</td></tr>
    <tr id="5"><td>5</td><td>Five</td><td>some text</td></tr>
    <tr id="6"><td>6</td><td>Six</td><td>some text</td></tr>
</table>{% endhighlight %}

To add in the &#8220;draggability&#8221; all we need to do is add a line to the `$(document).ready(...)` function  
as follows:

{% highlight html %}<script type="text/javascript">
$(document).ready(function() {
    <span class="comment">// Initialise the table</span>
    $("#table-1").tableDnD();
});
</script>{% endhighlight %}

In the example above we&#8217;re not setting any parameters at all so we get the default settings. There are a number of parameters you can set in order to control the look and feel of the table and also to add custom behaviour on drag or on drop. The parameters are specified as a map in the usual way and are described below:

onDragStyle
:   This is the style that is assigned to the row during drag. There are limitations to the styles that can be associated with a row (such as you can&#8217;t assign a border—well you can, but it won&#8217;t be displayed). (So instead consider using `onDragClass`.) The CSS style to apply is specified as a map (as used in the jQuery `css(...)` function).

onDropStyle
:   This is the style that is assigned to the row when it is dropped. As for onDragStyle, there are limitations to what you can do. Also this replaces the original style, so again consider using onDragClass which is simply added and then removed on drop.

onDragClass
:   This class is added for the duration of the drag and then removed when the row is dropped. It is more flexible than using onDragStyle since it can be inherited by the row cells and other content. The default is class is `tDnD_whileDrag`. So to use the default, simply customise this CSS class in your stylesheet.

onDrop
:   Pass a function that will be called when the row is dropped. The function takes 2 parameters: the table and the row that was dropped. You can work out the new order of the rows by using  
    `table.tBodies[0].rows`.

onDragStart
:   Pass a function that will be called when the user starts dragging. The function takes 2 parameters: the table and the row which the user has started to drag.

scrollAmount
:   This is the number of pixels to scroll if the user moves the mouse cursor to the top or bottom of the window. The page should automatically scroll up or down as appropriate (tested in IE6, IE7, Safari, FF2, FF3 beta)

This second table has has an onDrop function applied as well as an onDragClass. The javascript to set this up is as follows:

{% highlight javascript %}$(document).ready(function() {

	// Initialise the first table (as before)
	$("#table-1").tableDnD();

	// Make a nice striped effect on the table
	$("#table-2 tr:even').addClass('alt')");

	// Initialise the second table specifying a dragClass and an onDrop function that will display an alert
	$("#table-2").tableDnD({
	    onDragClass: "myDragClass",
	    onDrop: function(table, row) {
            var rows = table.tBodies[0].rows;
            var debugStr = "Row dropped was "+row.id+". New order: ";
            for (var i=0; i<rows.length; i++) {
                debugStr += rows[i].id+" ";
            }
	        $(#debugArea).html(debugStr);
	    },
		onDragStart: function(table, row) {
			$(#debugArea).html("Started dragging row "+row.id);
		}
	});
});{% endhighlight %}

<div class="tableDemo">
  <div id="debugArea" style="float: right; width:45%">
  </div>
  
  <table id="table-2" cellspacing="0" cellpadding="2" style="width:50%">
    <tr id="2.1">
      <td>
        1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        <input type="text" name="one" value="one" />
      </td>
    </tr>
    
    <tr id="2.2">
      <td>
        2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        <input type="text" name="two" value="two" />
      </td>
    </tr>
    
    <tr id="2.3">
      <td>
        3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        <input type="text" name="three" value="three" />
      </td>
    </tr>
    
    <tr id="2.4">
      <td>
        4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        <input type="text" name="four" value="four" />
      </td>
    </tr>
    
    <tr id="2.5">
      <td>
        5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        <input type="text" name="five" value="five" />
      </td>
    </tr>
    
    <tr id="2.6">
      <td>
        6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        <input type="text" name="six" value="six" />
      </td>
    </tr>
    
    <tr id="2.7">
      <td>
        7
      </td>
      
      <td>
        Seven
      </td>
      
      <td>
        <input type="text" name="seven" value="7" />
      </td>
    </tr>
    
    <tr id="2.8">
      <td>
        8
      </td>
      
      <td>
        Eight
      </td>
      
      <td>
        <input type="text" name="eight" value="8" />
      </td>
    </tr>
    
    <tr id="2.9">
      <td>
        9
      </td>
      
      <td>
        Nine
      </td>
      
      <td>
        <input type="text" name="nine" value="9" />
      </td>
    </tr>
    
    <tr id="2.10">
      <td>
        10
      </td>
      
      <td>
        Ten
      </td>
      
      <td>
        <input type="text" name="ten" value="10" />
      </td>
    </tr>
    
    <tr id="2.11">
      <td>
        11
      </td>
      
      <td>
        Eleven
      </td>
      
      <td>
        <input type="text" name="eleven" value="11" />
      </td>
    </tr>
    
    <tr id="2.12">
      <td>
        12
      </td>
      
      <td>
        Twelve
      </td>
      
      <td>
        <input type="text" name="twelve" value="12" />
      </td>
    </tr>
    
    <tr id="2.13">
      <td>
        13
      </td>
      
      <td>
        Thirteen
      </td>
      
      <td>
        <input type="text" name="thirteen" value="13" />
      </td>
    </tr>
    
    <tr id="2.14">
      <td>
        14
      </td>
      
      <td>
        Fourteen
      </td>
      
      <td>
        <input type="text" name="fourteen" value="14" />
      </td>
    </tr>
  </table>
</div>

## What to do afterwards?

Generally once the user has dropped a row, you need to inform the server of the new order. To do this, we&#8217;ve added a method called `serialise()`. It takes no parameters but knows the current table from the context. The method returns a string of the form `<em>tableId</em>[]=<em>rowId1</em>&<em>tableId</em>[]=<em>rowId2</em>&<em>tableId</em>[]=<em>rowId3</em>...`  
You can then use this as part of an Ajax load.

This third table demonstrates calling the serialise function inside onDrop (as shown below). It also demonstrates the &#8220;nodrop&#8221; class on row 3 and &#8220;nodrag&#8221; class on row 5, so you can&#8217;t pick up row 5 and  
you can&#8217;t drop any row on row 3 (but you can drag it).

{% highlight javascript %}$('#table-3').tableDnD({
        onDrop: function(table, row) {
            alert($.tableDnD.serialize());
        }
    });{% endhighlight %}

<div class="tableDemo">
  <div id="AjaxResult" style="float: right; width: 30%; border: 1px solid silver; padding: 4px; font-size: 90%;">
    <h3>
      Ajax result
    </h3>
    
    <p>
      Drag and drop in this table to test out serialise and using JQuery.load()
    </p>
  </div>
  
  <table id="table-3" cellspacing="0" cellpadding="2" style="width: 68%">
    <tr id="3.1">
      <td>
        1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        <input type="text" name="one" value="one" />
      </td>
    </tr>
    
    <tr id="3.2">
      <td>
        2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        <input type="text" name="two" value="two" />
      </td>
    </tr>
    
    <tr id="3.3" class="nodrop">
      <td>
        3
      </td>
      
      <td>
        Three (Can&#8217;t drop on this row)
      </td>
      
      <td>
        <input type="text" name="three" value="three" />
      </td>
    </tr>
    
    <tr id="3.4">
      <td>
        4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        <input type="text" name="four" value="four" />
      </td>
    </tr>
    
    <tr id="3.5" class="nodrag">
      <td>
        5
      </td>
      
      <td>
        Five (Can&#8217;t drag this row)
      </td>
      
      <td>
        <input type="text" name="five" value="five" />
      </td>
    </tr>
    
    <tr id="3.6">
      <td>
        6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        <input type="text" name="six" value="six" />
      </td>
    </tr>
  </table>
</div>

This table has multiple TBODYs. The functionality isn&#8217;t quite working properly. You can only drag the rows inside their own TBODY, you can&#8217;t drag them outside it. Now this might or might not be what you want, but unfortunately if you then drop a row outside its TBODY you get a Javascript error because inserting after a sibling doesn&#8217;t work. This will be fixed in the next version. The header rows all have the classes &#8220;nodrop&#8221; and &#8220;nodrag&#8221; so that they can&#8217;t be dragged or dropped on.

<div class="tableDemo">
  <table id="table-4" cellspacing="0" cellpadding="2">
    <tr id="4.0" class="nodrop nodrag">
      <th>
        H1
      </th>
      
      <th>
        H2
      </th>
      
      <th>
        H3
      </th>
    </tr>
    
    <tr id="4.1">
      <td>
        4.1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        <input type="text" name="one" value="one" />
      </td>
    </tr>
    
    <tr id="4.2">
      <td>
        4.2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        <input type="text" name="two" value="two" />
      </td>
    </tr>
    
    <tr id="4.3">
      <td>
        4.3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        <input type="text" name="three" value="three" />
      </td>
    </tr>
    
    <tr id="4.4">
      <td>
        4.4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        <input type="text" name="four" value="four" />
      </td>
    </tr>
    
    <tr id="4.5">
      <td>
        4.5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        <input type="text" name="five" value="five" />
      </td>
    </tr>
    
    <tr id="4.6">
      <td>
        4.6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        <input type="text" name="six" value="six" />
      </td>
    </tr>
    
    <tr id="5.0" class="nodrop nodrag">
      <th>
        H1
      </th>
      
      <th>
        H2
      </th>
      
      <th>
        H3
      </th>
    </tr>
    
    <tr id="5.1">
      <td>
        5.1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        <input type="text" name="one" value="one" />
      </td>
    </tr>
    
    <tr id="5.2">
      <td>
        5.2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        <input type="text" name="two" value="two" />
      </td>
    </tr>
    
    <tr id="5.3">
      <td>
        5.3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        <input type="text" name="three" value="three" />
      </td>
    </tr>
    
    <tr id="5.4">
      <td>
        5.4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        <input type="text" name="four" value="four" />
      </td>
    </tr>
    
    <tr id="5.5">
      <td>
        5.5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        <input type="text" name="five" value="five" />
      </td>
    </tr>
    
    <tr id="5.6">
      <td>
        5.6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        <input type="text" name="six" value="six" />
      </td>
    </tr>
    
    <tr id="6.0" class="nodrop nodrag">
      <th>
        H1
      </th>
      
      <th>
        H2
      </th>
      
      <th>
        H3
      </th>
    </tr>
    
    <tr id="6.1">
      <td>
        6.1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        <input type="text" name="one" value="one" />
      </td>
    </tr>
    
    <tr id="6.2">
      <td>
        6.2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        <input type="text" name="two" value="two" />
      </td>
    </tr>
    
    <tr id="6.3">
      <td>
        6.3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        <input type="text" name="three" value="three" />
      </td>
    </tr>
    
    <tr id="6.4">
      <td>
        6.4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        <input type="text" name="four" value="four" />
      </td>
    </tr>
    
    <tr id="6.5">
      <td>
        6.5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        <input type="text" name="five" value="five" />
      </td>
    </tr>
    
    <tr id="6.6">
      <td>
        6.6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        <input type="text" name="six" value="six" />
      </td>
    </tr>
  </table>
</div>

The following table demonstrates the use of the default regular expression. The rows have IDs of the form table5-row-1, table5-row-2, etc., but the regular expression is `/[^-]*$/` (this is the same as used in the [NestedSortable][4] plugin for consistency). This removes everything before and including the last hyphen, so the serialised string just has 1, 2, 3 etc. You can replace the regular expression by setting the `serializeRegexp` option, you can also just set it to null to stop this behaviour.

{% highlight javascript %}$('#table-5').tableDnD({
        onDrop: function(table, row) {
            alert($('#table-5').tableDnDSerialize());
        },
        dragHandle: ".dragHandle"
    });{% endhighlight %}

<div class="tableDemo">
  <table id="table-5" cellspacing="0" cellpadding="2">
    <tr id="table5-row-1">
      <td class="dragHandle">
      </td>
      
      <td>
        1
      </td>
      
      <td>
        One
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="table5-row-2">
      <td class="dragHandle">
      </td>
      
      <td>
        2
      </td>
      
      <td>
        Two
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="table5-row-3">
      <td class="dragHandle">
      </td>
      
      <td>
        3
      </td>
      
      <td>
        Three
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="table5-row-4">
      <td class="dragHandle">
      </td>
      
      <td>
        4
      </td>
      
      <td>
        Four
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="table5-row-5">
      <td class="dragHandle">
      </td>
      
      <td>
        5
      </td>
      
      <td>
        Five
      </td>
      
      <td>
        some text
      </td>
    </tr>
    
    <tr id="table5-row-6">
      <td class="dragHandle">
      </td>
      
      <td>
        6
      </td>
      
      <td>
        Six
      </td>
      
      <td>
        some text
      </td>
    </tr>
  </table>
</div>

In fact you will notice that I have also set the dragHandle on this table. This has two effects: firstly only the cell with the drag handle class is draggable and secondly it doesn&#8217;t automatically add the `cursor: move` style to the row (or the drag handle cell), so you are responsible for setting up the style as you see fit.

Here I&#8217;ve actually added an extra effect which adds a background image to the first cell in the row whenever you enter it using the jQuery `hover` function as follows:

{% highlight javascript %}$("#table-5 tr").hover(function() {
          $(this.cells[0]).addClass('showDragHandle');
    }, function() {
          $(this.cells[0]).removeClass('showDragHandle');
    });{% endhighlight %}

This provides a better visualisation of what you can do to the row and where you need to go to drag it (I hope).

 [1]: http://jquery.com
 [2]: /2007/07/dragging-and-dropping-table-rows-in-javascript
 [3]: https://github.com/isocra/TableDnD/tree/master/stable
 [4]: http://plugins.jquery.com/project/NestedSortable
