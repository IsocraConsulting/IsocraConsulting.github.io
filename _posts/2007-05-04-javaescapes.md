---
title: Localisation in Java
author: denish
layout: post
permalink: /2007/05/javaescapes/
categories:
  - Articles
  - Java
tags:
  - Java
  - Localisation
  - UTF8
---
We&#8217;ve recently had a problem where we wanted to produce a website in multiple languages including Russian, Czech, Romanian, and other eastern European languages. No problems, we thought, we can just use Java properties files and the `fmt:message` JSTL tags.<!--more-->

Unfortunately, it&#8217;s not quite that simple because properties files cannot be UTF-8, so getting the source languages into properties files proved to be nigh on impossible. There is a program `native2ascii` provided in the Java JDK, but this only copes with files in the current native locale, it can&#8217;t cope with UTF-8 files either.

So, in the end we wrote a little utility to do it for us. It&#8217;s actually very straight forward, just a single  
function does the trick:

<pre class="brush:java">String encodedString(String line) {
        StringBuffer out = new StringBuffer();
        line = toUTF8(line);
        char[] chars = line.toCharArray();
        for (int i = 0; i &lt; chars.length; i++) {
            char aChar = chars[i];
            if (aChar &gt; 127) {
                out.append(String.format("\u%04x", new Object[]{new Long((long) aChar)}));
            } else {
                out.append(aChar);
            }
        }
        return out.toString();
    }</pre>

All we have to do is go through each character and if it is larger than 127, then we just write it out  
in the `u<em>xxx</em>` format.

You can take this code and call it from a loop that reads in a text file and then outputs it again (though you might have to tell Java what the encoding of the file is).

We decided to put it in a web page. Here we found another wrinkle, the string that you get when you call `request.getParameter(...)` is encoded as ISO-8859-1. So you have to take the request parameter and convert it to UTF-8 as follows:

<pre>String source = new String(request.getParameter("source").getBytes("ISO-8859-1"), "UTF-8");</pre>

However, to save you the hassle, we&#8217;ve decided to put our web-based converter up on our website as a free service. So if you need to convert some strings, [go to our converter!][1].

 [1]: /encoder/
