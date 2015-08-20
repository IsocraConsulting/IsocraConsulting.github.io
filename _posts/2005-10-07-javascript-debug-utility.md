---
title: Javascript Debug Utility
author: denish
layout: post
permalink: /2005/10/javascript-debug-utility/
categories:
  - Articles
  - Web development
tags:
  - Debugging
  - Javascript
  - Web development
---
**Note:** See the [May 2008][1] update at the bottom of this article.

One of the problems when writing JavaScript is that you don&#8217;t have a development environment with a debugger to find out what&#8217;s going on. For Microsoft platforms, there is the [ Microsoft Script Debugger][2] which provides some help, but often people resort to just sprinkling `alert("You are here")` around the code to find out what&#8217;s going on. The problems with this approach are that you can&#8217;t easily switch them on and off without going through and commenting out individual calls; the fact that you get a dialog box up which needs to be accepted by the user can affect time-sensitive code, so you might not be able to debug it using this method; and finally it&#8217;s difficult to compare values from one `alert` to the next since the text just comes up in a dialog box.

Here we present three functions that allow you to put debug calls in your code and switch them on and off as needed. <!--more-->The debug calls go to a debug window, the advantage of this is that you can treat it just like a log file, you can save it away, you can compare the results of calls in a loop for example, and you can output time-sensitive information (though obviously writing to the window is going to have a small performance hit on your code).

{% highlight javascript %}
// Show the debug window
function showDebug() {
    window.top.debugWindow =
    window.open("",
        "Debug",
        "left=0,top=0,width=300,height=700,scrollbars=yes,"
        +"status=yes,resizable=yes");

    window.top.debugWindow.opener = self;

    // open the document for writing
    window.top.debugWindow.document.open();
    window.top.debugWindow.document.write(
        "<HTML><HEAD><TITLE>Debug Window</TITLE></HEAD><BODY><PRE>n");
}
{% endhighlight %}

The first function `showDebug()` opens a new window (if you are using a pop-up blocker such as the excellent [Google Toolbar][3] you&#8217;ll need to allow pop-ups for the site while debugging). Rather than using a variable, we&#8217;ve used a property of `window.top`. The nice thing about this is that you don&#8217;t have to worry that your variable is going to clash with another one and you don&#8217;t have to worry about scope (whether the variable is declared in a frame or in the parent). By using `window.top`, wherever you call the code from, it will access the same value so the code works fine in pages with or without frames.

The parameters to `window.open([URL] [, name] [, features] [, replace])` are:

`URL`
:   The URL to display in the window. We leave this as `""` because  
    we are going to write into the window ourselves.

`name`
:   This is name of the window and can be used as the value of the `target` property of `<A href=...` links. The name doesn&#8217;t really matter here because we&#8217;re  
    going to access the window directly.

`features`
:   This is a string of comma separated features that control what the window will be like, you can set the position and the size, say whether it&#8217;s resizable, whether it has a menu bar etc. For full information on how this works for Internet Explorer, have a look at the documentation on the [MSDN website][4].

`replace`
:   This specifies if when a new URL is loaded into the same window, it replaces the entry in the window&#8217;s history list or not. Since it&#8217;s optional and not relevant, we don&#8217;t specify it in our call.

Once we&#8217;ve created and opened the debug window, we then get hold of the document and start writing to it. We could have made it into a text document which is useful if you want to debug HTML that you&#8217;re generating from JavaScript, but we&#8217;ve opted here to make it into an HTML document which is nice since you can then put HTML tags into the debug messages (and you can always get at the raw debug by doing `View Source` anyway).

{% highlight javascript %}
// If the debug window exists, then write to it
function debug(text) {
    if (window.top.debugWindow && ! window.top.debugWindow.closed) {
        window.top.debugWindow.document.write(text+"n");
    }
}
{% endhighlight %}

The next function `debug` actually writes a debug message to the debug window. First we check to see if there is a window and that it&#8217;s not closed. If it is there and open, we simply call `document.write` and write in whatever text we&#8217;re called with followed by a carriage return so that it&#8217;s easy to read.

To use it in your code, simply add `debug("The value of my variable is "+var)` or whatever you need to print out.

We could do all sorts of other things. For example we could get the current time and put a timestamp, we could append `<BR>` at the end of each line so that the debug messages appear on different lines, or we could implement some concept of leve of debug by passing in an extra parameter. We&#8217;re sure you get the idea!

{% highlight javascript %}
// If the debug window exists, then close it
function hideDebug() {
    if (window.top.debugWindow && ! window.top.debugWindow.closed) {
        window.top.debugWindow.close();
        window.top.debugWindow = null;
    }
}
{% endhighlight %}


Finally, the last function `hideDebug` simply closes the window and sets `window.top.debugWindow` to null so that we don&#8217;t try and write debug messages to it after this call.

There are two ways of using this code: you can either just put a call to `showDebug` at the beginning of your code and then simply comment it out when you don&#8217;t want the debug, or you can have a debug button on your page somewhere that switches on and off the debug as necessary. Obviously even if you don&#8217;t call `showDebug`, there&#8217;s still going to be a performance hit for every call to `debug` since it will call the function and then inside the function will notice there&#8217;s nothing to do and return. You can improve this slightly by calling it as follows:

{% highlight javascript %}
if (window.top.debugWindow) debug("the value of my variable is "+var);
{% endhighlight %}

But unless the performance of your script is unacceptable to the user, we&#8217;d recommend using it in the simplest form to make the code as readable as possible.

We&#8217;ve tested the code with Microsoft&#8217;s Internet Explorer, Mozilla Firefox and Opera (see the note on the [demo page][5]).

## <a name="may08">May 2008 Update</a>

I wrote this a long, long time ago and fortunately the word has moved on. There are now a number of different options:

If you&#8217;re using Internet Explorer, then check out Microsoft&#8217;s free [Web Developer Express][7]. It has WYSIWYG HTML editing as well as syntax highlighting for HTML and Javascript and most importantly a very good Javascript debugger. Then look at this excellent [Javascript debugging tutorial at BernieCode][8].

If you&#8217;re using Firefox, then you *must* use the absolutely excellent [Firebug][9] plugin. Not only does it provide excellent Javascript debugging including being able to set breakpoints, inspect values and change them, but it also provides a fantastic HTML and CSS inspector, even allowing you to change HTML or CSS on the fly and see what it looks like. It also provides the ability to [write to the console][10], so you can simply use `console.log(2, "some text", variable)`. You can even pass a printf style format string.

Finally, if you are using Opera, then you can `opera.postError("some text")`. For information on this, see [How to debug JavaScript problems with Opera][11].

 [1]: /2005/10/javascript-debug-utility/#may08
 [2]: http://msdn.microsoft.com/library/default.asp?url=/downloads/list/webdev.asp
 [3]: http://toolbar.google.com/
 [4]: http://msdn.microsoft.com/library/default.asp?url=/workshop/author/dhtml/reference/methods/open_0.asp
 [5]: /articles/jsDebugDemo.html
 [6]: /articles/debug.js.zip
 [7]: www.microsoft.com/express/vwd/
 [8]: http://www.berniecode.com/blog/2007/03/08/how-to-debug-javascript-with-visual-web-developer-express/
 [9]: http://www.getfirebug.com/
 [10]: http://www.getfirebug.com/logging.html
 [11]: http://dev.opera.com/articles/view/how-to-debug-javascript-problems-with-op/
