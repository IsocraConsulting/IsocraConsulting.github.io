---
title: Debugging Javascript in Firefox
author: denish
layout: post
permalink: /2006/03/debugging-javascript-in-firefox/
categories:
  - Blog Entries
  - Web development
tags:
  - Firefox
  - Web development
---
I&#8217;ve recently been trying to make some of my scripts work with Firefox 1.5 (from IE) and I was looking for some debugging help to set me on the right track. I found this tool which seems to work very well: <http://getahead.ltd.uk/ajax/venkman>. 

It&#8217;s an updated version of Venkman fixed to work with Firefox 1.5.

One of the things it helped me find is that whereas in IE if I ask a table cell (TD) for the nextSibling and get the next TD, in Firefox you always seem to get a text node first and then the next TD. Similarly for getting the next and previous rows.