---
title: Dragging and dropping table rows in Javascript
author: denish
layout: post
permalink: /2007/07/dragging-and-dropping-table-rows-in-javascript/
categories:
  - Articles
  - JavaScript
  - Web development
tags:
  - 'Drag &amp; Drop'
  - Javascript
  - Web development
---
<p id="summary">
  <strong>Summary:</strong> This article tells you how to implement drag and drop for HTML tables in Javascript. You can download the source <a href="/articles/tablednd.js.zip">here</a> and play with the demo <a href="#demo">here</a>.
</p>

**Updated:** now copes with multiple tables on the same page, non-drag and non-drop rows (such as headers) and embedding form elements. Also see my [jQuery plug-in][1] which does much more.<!--more-->

There are many articles on implementing drag and drop in Javascript and many excellent frameworks and libraries that provide you with everything you need. In fact I use [Script.aculo.us][2] quite a bit. However, I haven&#8217;t found anything much that tells you how to re-order rows in a table. Table rows are different from other elements normally used for drag and drop such as list items or divs because they can&#8217;t be moved about in the same way. It wouldn&#8217;t make sense to have absolute positioning on table rows—they sort of have to be in the table. There are also limitations on the styles you can put on rows, you can&#8217;t have a border round them for example. So, the solution to this problem is slightly different from other drag and drop mechanisms. Here&#8217;s and example, try dragging the rows about:

<a title="demo" name="demo"></a>

<div class="tableDemo">
  <div id="debug" style="float: right;">
  </div>
  
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

## Drag and Drop basics

I won&#8217;t spend a great deal of time explaining the basics of drag and drop. Instead I&#8217;ll point you at [How to Drag and Drop in Javascript][3]. This article provides a good explanation including code that you can download and use.

The steps you need are:

  1. capture the mouse move event for the whole document so you can track where the mouse is.
  2. capture the onmouseup event for the whole document so we know when our dragged thing is dropped.
  3. add onmousedown functions for each draggable row so that we know which row is being dragged.

In order to make this neat and reusable I have encapsulated the required data in a class (we&#8217;re also going to add functionality in here later):

<pre class="brush:javascript">function TableDnD() {
    /** Keep hold of the current drag object if any */
    this.dragObject = null;
    /** The current mouse offset */
    this.mouseOffset = null;
    /** The current table */
    this.table = null;
    /** Remember the old value of Y so that we don't do too much processing */

    this.oldY = 0;

    // rest of the code goes here...
}</pre>

The class has instance variables for the currently dragged object, the current mouse offset, the current table, and the oldY so that we can detect if we&#8217;re moving up or down. We will create one TableDnD object for each table for which we want to enable to drag and drop

## Setting up the event handlers

Now we need to capture the `document.onmousemove` and `document.onmouseup` events so that we can track where the user drags and drops the row. The first version that I developed captured the events insidethe TableDnD object, but this doesn&#8217;t work if you want multiple tables on the same page. So we have to abstract this out and have global handlers for the whole page. We also therefore need to know which TableDnD object we&#8217;re currently concerned with (in other words which one, if any, initiated the drag). So, here are the event handlers event handlers we need (and a global variable to keep track):

<pre class="brush:javascript">/** Keep hold of the current table being dragged */
var currenttable = null;

/** Capture the onmousemove so that we can see if a row from the current
 *  table if any is being dragged.
 * @param ev the event (for Firefox and Safari, otherwise we use window.event for IE)
 */
document.onmousemove = function(ev){
    if (currenttable && currenttable.dragObject) {
        ev   = ev || window.event;
        var mousePos = currenttable.mouseCoords(ev);
        var y = mousePos.y - currenttable.mouseOffset.y;
        if (y != currenttable.oldY) {
            // work out if we're going up or down...
            var movingDown = y &gt; currenttable.oldY;
            // update the old value
            currenttable.oldY = y;
            // update the style to show we're dragging

            currenttable.dragObject.style.backgroundColor = "#eee";
            // If we're over a row then move the dragged row to there so that the user sees the
            // effect dynamically
            var currentRow = currenttable.findDropTargetRow(y);
            if (currentRow) {
                if (movingDown && currenttable.dragObject != currentRow) {
                    currenttable.dragObject.parentNode.insertBefore(currenttable.dragObject, currentRow.nextSibling);
                } else if (! movingDown && currenttable.dragObject != currentRow) {
                    currenttable.dragObject.parentNode.insertBefore(currenttable.dragObject, currentRow);
                }
            }
        }

        return false;
    }
}

// Similarly for the mouseup
document.onmouseup = function(ev){
    if (currenttable && currenttable.dragObject) {
        var droppedRow = currenttable.dragObject;
        // If we have a dragObject, then we need to release it,

        // The row will already have been moved to the right place so we just reset stuff
        droppedRow.style.backgroundColor = 'transparent';
        currenttable.dragObject   = null;
        // And then call the onDrop method in case anyone wants to do any post processing
        currenttable.onDrop(currenttable.table, droppedRow);
        currenttable = null; // let go of the table too
    }
}</pre>

In the `onmousemove` method itself, we first of all check to see if we have a current table, and if so, does that have a dragObject. If not, we don&#8217;t need to do anything. If we do have a dragObject, then we need to get the event. In Internet Explorer, the event is global and accessible using `window.event`, in Firefox and other browsers it is passed in as a parameter, so we need to check for both cases. Once we have that, we can get the mouse coordinates (again code to follow), and check the y position. Because we&#8217;re only dragging rows, we&#8217;re only interested in the vertical direction, if the y value hasn&#8217;t changed, then we don&#8217;t need to do anything (we could also put in a threshold here so we don&#8217;t worry about small movements).

Assuming that y has changed, we can work out whether it&#8217;s an upwards or downwards direction by comparing it with the old value (you&#8217;ll see why we need this in a moment). Then we can set the background colour of the dragObject to something to make it obvious it is being dragged (we&#8217;re fairly limited as to what styles we can apply to rows—a neater approach would be to add and subtract a class, like that the style could be controlled by a stylesheet rather than code and it would be inherited by the constituent cells). Next we find out which row the mouse is currently over (again we only really need to worry about the y coordinate, we&#8217;re not really worried if the mouse strays left or right outside the table—though we could change for that if needed).

Now we know which row the mouse is over, we want to move our row to be before or after the current row depending on whether we&#8217;re moving up or down. After a quick check to make sure that we&#8217;re not moving it to where it already is, we use `parentNode.insertBefore(...)` to move the row. If we&#8217;re moving down, we get the nextSibling and insert before that, otherwise we just insert before the current row.

If we do move the row, then we return false from the event handler so that no other related event fires and default handling isn&#8217;t engaged.

The `onmouseup` method is much more straight forward. All we need to do is reset the style and then forget the dragObject and the current table.

That&#8217;s what happens when we&#8217;re actually dragging something, but how do we initiate the drag? We need to capture the mouse down event on the rows that we want to drag. Back in our TableDnD class we add an `init` method which takes the table as a parameter and sets everything up:

<pre class="brush:javascript">/** Initialise the drag and drop by capturing mouse move events */

    this.init = function(table) {
        this.table = table;
        var rows = table.tBodies[0].rows; //getElementsByTagName("tr")
        for (var i=0; i&lt;rows.length; i++) {
            // John Tarr: added to ignore rows for which the NoDrag attribute is set
            var nodrag = rows[i].getAttribute("NoDrag")
            if (nodrag == null || nodrag == "undefined") { // There is no NoDrag attribute so make draggable
                this.makeDraggable(rows[i]);
            }
        }
    }</pre>

We get passed in the table whose rows we want to be able to drag and drop, so we remember that, then we go through all the rows in the table body and make them &#8220;draggable&#8221; (code for this coming soon). John Tarr contacted me to say that he needed to be able to control which rows were draggable and which not (for example headers shouldn&#8217;t be draggable). So he added a simple NoDrag attribute which can be used to switch off &#8220;draggability&#8221;.

Of course you might want to do something with the table once row has been dropped, so I&#8217;ve made the method call an onDrop method passing it the table and the dropped row. The default implementation does nothing, but you can redefine it to do whatever you need to (in fact in my current project I use this to make an Ajax call to let the server know the new order of the rows).

<pre class="brush:javascript">/** This function is called when you drop a row, so redefine it in your code
        to do whatever you want, for example use Ajax to update the server */
    this.onDrop = function(table, droppedRow) {
        // Do nothing for now
    }</pre>

## Getting the coordinates

Now we need some methods that get the mouse position from an event:

<pre class="brush:javascript">/** Get the position of an element by going up the DOM tree and adding up all the offsets */
    this.getPosition = function(e){
        var left = 0;
        var top  = 0;

        while (e.offsetParent){
            left += e.offsetLeft;
            top  += e.offsetTop;
            e     = e.offsetParent;
        }

        left += e.offsetLeft;
        top  += e.offsetTop;

        return {x:left, y:top};
    }

    /** Get the mouse coordinates from the event (allowing for browser differences) */
    this.mouseCoords = function(ev) {
        if(ev.pageX || ev.pageY){
            return {x:ev.pageX, y:ev.pageY};
        }
        return {
            x:ev.clientX + document.body.scrollLeft - document.body.clientLeft,
            y:ev.clientY + document.body.scrollTop  - document.body.clientTop
        };
    }

    <span class="comment">/** Given a target element and a mouse event, get the mouse offset from that element.
        To do this we need the element's position and the mouse position */</span>

    this.getMouseOffset = function(target, ev){
        ev = ev || window.event; // In FireFox and Safari, we get passed the event, in IE we need to get it

        var docPos    = this.getPosition(target);
        var mousePos  = this.mouseCoords(ev);
        return {x:mousePos.x - docPos.x, y:mousePos.y - docPos.y};
    }</pre>

The first method, *getPosition*, takes an element and walks up the DOM using offsetParent to add up all the offsets to get the absolute position of the element. It returns the position as an object with x and y instance variables.

The next method, *mouseCoords* takes an event and extracts the coordinates from it. Firefox and other browsers use `event.pageX` and `event.pageY` to store the position, so we can just return this. Internet Explorer however uses `event.clientX` and `event.clientY`. What&#8217;s more is that these values are for the current window, not the position from the top of the page, so in order to be able to map these to the same values as pageX and pageY, we have to add in the current scroll position of the document.

The final method in this trio is *getMouseOffset* this takes a target element and an event and works out where the mouse is in relation to the element. It processes the event in the same way as we saw before to allow for browser differences and the calls the two methods above to get the positions and returns the relative position of the mouse.

## Which rows?

We need two more methods to complete our class:

<pre class="brush:javascript">/** Take an item and add an onmousedown method so that we can make it draggable */
    this.makeDraggable = function(item){
        if(!item) return;
        var self = this; // Keep the context of the TableDnd inside the function
        item.onmousedown = function(ev){
            // get the event source in a browser independent way
            var target = getEventSource(ev);
            // if it's an INPUT or a SELECT, then let the event bubble through, don't do a drag
            if (target.tagName == 'INPUT' || target.tagName == 'SELECT') return true;
            self.dragObject  = this;
            self.mouseOffset = self.getMouseOffset(this, ev);
            return false;
        }
        item.style.cursor = "move";
    }

    /** We're only worried about the y position really, because we can only move rows up and down */

    this.findDropTargetRow = function(y) {
        var rows = this.table.tBodies[0].rows;
        for (var i=0; i&lt;rows.length; i++) {
            var row = rows[i];
            // John Tarr added to ignore rows that I've added the NoDrop attribute to (Header rows)
            var nodrop = row.getAttribute("NoDrop");
            if (nodrop == null || nodrop == "undefined") {  //There is no NoDrop attribute on rows I want to drag
                var rowY    = this.getPosition(row).y;
                var rowHeight = parseInt(row.offsetHeight)/2;
                if (row.offsetHeight == 0) {
                    rowY = this.getPosition(row.firstChild).y;
                    rowHeight = parseInt(row.firstChild.offsetHeight)/2;
                }
                // Because we always have to insert before, we need to offset the height a bit
                if ((y &gt; rowY - rowHeight) && (y &lt; (rowY + rowHeight))) {
                    // that's the row we're over

                    return row;
                }
            }
        }
        return null;
    }</pre>

The first, *makeDraggable* is called from the `init` function for each row in the table. It defines an `onmousedown` event handler to set the dragObject and the initial mouse offset (so we can track movements relative to the initial drag position). Now inside the `onmousedown` event handler that we are adding to each row, we want to be able to access the current TableDnD object, we can&#8217;t use `this` because it will be changed to the current object when the event handler is called, so instead we have a variable `self` which we bind to `this` outside the handler but which retains it&#8217;s value inside. Now the handler can refer directly to the methods and data on the TableDnD object. Inside the event handler we also need to check to see what the event source is, because if we capture and consume the `onmousedown` event for form elements, then the user won&#8217;t be able to click in them and type, or select. So in that case we have to just bubble it up by returning `true`.

As well as setting the `onmousedown` event handler, we also set the row&#8217;s cursor style to &#8220;move&#8221; so that the user can see it&#8217;s draggable.

The last method, *findDropTargetRow* works out the current row under the mouse. This is called as we move the mouse and is used to dynamically move the row so that the user can see what is happening. It simply iterates through the rows, getting the top and height of the rows and checks to see if the mouse is on it or not (in fact I displace the position by half a row to make it feel right when dragging, otherwise it&#8217;s very quick in one direction and seems &#8220;heavy&#8221; in the other direction—try different values to see what I mean!).

Again, John Tarr suggested that we should support no-drop zones too, so if the row has NoDrop set to true, then we just return null and the user can&#8217;t drop the row there

There is a browser problem however. Getting the offsetHeight for a row works fine in Internet Explorer (6 and 7) and Firefox (2.0.x), however Safari returns 0 for rows! Fortunately you can get the offsetHeight of a cell instead, so the code uses the row&#8217;s firstChild if the offsetHeight is zero. The same thing seems to happen for the rowY, so again I use the first cell.

In fact there was still a problem with Safari because the row&#8217;s offsetTop is also 0 (or sometimes a small number&#8211;presumably from the style). Thanks to Luis Chato for pointing this out and pointing me to [this description of the problem][4]. The same answer works again though. If the e.offsetHeight == 0 for the selected element in getPosition then we&#8217;ll just get the firstChild and use that instead. This works for all the browsers I&#8217;ve tested so far.

As well as the class we need one more global method to get the event source. IE and Firefox (and the others) do this in different ways. We could add this as an instance method on the TableDnD class, but because there is no TableDnD context needed, I decided to just make it a globally accessible method. Here it is:

<pre class="brush:javascript">/** get the source element from an event in a way that works for IE and Firefox and Safari
 * @param evt the source event for Firefox (but not IE--IE uses window.event) */

function getEventSource(evt) {
    if (window.event) {
        evt = window.event; // For IE
        return evt.srcElement;
    } else {
        return evt.target; // For Firefox
    }
}</pre>

## Putting it all together

You can download the complete class and other methods from the [resources][5] below and then link to it from your web page. The next thing you need to do is to have a table, and then you can drag-enable the table by adding the following javascript either inline below the table HTML or in the `document.onload` event handler.

<pre class="brush:javascript">&lt;script type="text/javascript"&gt;
var table = document.getElementById('table-1');
var tableDnD = new TableDnD();
tableDnD.init(table);
&lt;/script&gt;</pre>

So all you need to do is create an instance of the TableDnD class, and then call `init` on it passing it the table you want to use. If you want to do something special when a row is dropped, you can add something like this:

<pre class="brush:javascript">// Redefine the onDrop so that we can display something
tableDnD.onDrop = function(table, row) {
    var rows = this.table.tBodies[0].rows;
    var debugStr = "rows now: ";
    for (var i=0; i&lt;rows.length; i++) {
        debugStr += rows[i].id+" ";
    }
    document.getElementById('debug').innerHTML = 'row['+row.id+'] dropped&lt;br&gt;'+debugStr;
}</pre>

In fact if you look at the source of this page you can see how I implemented the debugging information displayed as you drag rows in the demonstration table at the beginning of this article.

## Bells and whistles

Here&#8217;s a final example showing that you can have two separate tables on the same page, that you can support INPUTs  
SELECTs, and header rows which aren&#8217;t draggable or droppable:

<div class="tableDemo">
  <table id="table-2" cellspacing="0" cellpadding="2">
    <tr>
      <th>
        Label
      </th>
      
      <th>
        Value
      </th>
      
      <th>
        Input
      </th>
    </tr>
    
    <tr class="category">
      <td title="You can't drag or drop this row" colspan="3">
        Category 1
      </td>
    </tr>
    
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
        <input type="checkbox" name="two" value="two" /> Two
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
        <input type="radio" checked="checked" name="three" value="three" /> Three
      </td>
    </tr>
    
    <tr class="category">
      <td title="You can't drag this row, but you can drop other rows onto it" colspan="3">
        Category 2
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
        <select name="four"><option>4a</option><option>4b</option></select>
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
        <input type="radio" name="three" value="five" /> Five
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
  </table>
</div>

Note there is a problem with IE6 in that when you drag a row that has either a checkbox or a radio button in it, these get set back to their initial settings (well, it seems it&#8217;s a bit more complicated even than this&#8211;try it with the two radio buttons above). As far as I know this is fixed in IE7. If it&#8217;s crucial for you to support this, you would have to capture the state of the form elements on mouse down and then go through and reset them on mouse up. It only applies to radio buttons and check boxes on IE6, Safari and Firefox are fine, and select and text input tags are fine.

## <a name="resources"></a>Resources

  * [Download the javascript source][6] – the class ready for use in your web pages
  * [Script.aculo.us][2] – an excellent javascript library
  * [How to Drag and Drop in Javascript][3] – a good guide to how drag and drop works and how to do it for other types of element.

 [1]: /2008/02/table-drag-and-drop-jquery-plugin/
 [2]: http://script.aculo.us/
 [3]: http://www.webreference.com/programming/javascript/mk/column2/
 [4]: http://jacob.peargrove.com/blog/2006/technical/table-row-offsettop-bug-in-safari/
 [5]: #resources
 [6]: /articles/tablednd.js.zip