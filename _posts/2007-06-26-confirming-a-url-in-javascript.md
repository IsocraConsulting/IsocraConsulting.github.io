---
title: Confirming a URL in Javascript
author: denish
layout: post
permalink: /2007/06/confirming-a-url-in-javascript/
categories:
  - Blog Entries
  - Web development
tags:
  - Javascript
  - Web development
---
This is a little tip that I discovered a while ago but which I&#8217;m always forgetting, so I thought I&#8217;d post it here where I woudn&#8217;t lose it! Sometimes you&#8217;ll have a link on a web page which actually does something to a database, deletes the current thing for example. In those circumstances you want to have some sort of &#8220;Are you sure?&#8221; confirmation.

Javascript has a confirm dialog that you can use, but because you can use the return value of a method to determine event bubbling, you can use this really neatly to give optional links as follows:

```javascript
<a href="http://www.isocra.com" onclick="javascript:return confirm('Are you sure you want to go to our home page ?')">
Click here to go to home page</a>
```

And of course you can try it yourself: <a href="http://www.isocra.com" onclick="javascript:return confirm('Are you sure you want to go to our home page ?')">Click here to go to home page</a>.

If you click OK, then the confirm returns true and the link is followed. If you click Cancel, then confirm returns false and the event isn&#8217;t bubbled up to the href handler and so the link isn&#8217;t followed and you stay on the same page.
