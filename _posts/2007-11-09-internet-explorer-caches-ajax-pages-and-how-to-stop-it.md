---
title: Internet Explorer caches Ajax pages, and how to stop it
author: denish
layout: post
permalink: /2007/11/internet-explorer-caches-ajax-pages-and-how-to-stop-it/
categories:
  - Blog Entries
  - Web browsers
  - Web development
---
Caching pages leads to a speedier browsing experience and so is a good idea. Replacing information within a web page using Ajax similarly improves the user&#8217;s experience. The problem comes when the browser caches the information from the Ajax request. It&#8217;s a logical thing to do, but it&#8217;s not normally what the developer wants.

There are a number of ways round this. <!--more--> One way is to add an extra parameter to the request with the current time in milliseconds or something similar that will make each request unique. This works if you can control the page making the request, but can&#8217;t modify the source of the Ajax data.

If you can modify the code that generates the data for the Ajax request, then you can use the same mechanisms as you use to control normal page caching. For example in a JSP you can use:

```javascript
<%
// To ensure that IE doesn't cache the results of an Ajax call
response.setHeader("Cache-Control","no-store, no-cache, must-revalidate"); //HTTP 1.1
response.setHeader("Pragma","no-cache"); //HTTP 1.0
response.setDateHeader ("Expires", 0); //prevents caching at the proxy server
%>
````

This works for IE6 and IE7. Firefox and Safari don&#8217;t seem to cache Ajax requests.
