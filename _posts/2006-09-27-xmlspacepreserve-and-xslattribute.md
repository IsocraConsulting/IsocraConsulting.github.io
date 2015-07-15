---
title: 'xml:space=&#8221;preserve&#8221; and xsl:attribute'
author: denish
layout: post
permalink: /2006/09/xmlspacepreserve-and-xslattribute/
categories:
  - Blog Entries
  - Web development
tags:
  - XML
  - XSLT
---
Just a quick tip about xsl which I haven&#8217;t found anywhere else&#8230;

I wanted to generate some human (well me) readable XML using XSL and found that if you add *xml:space=&#8221;preserve&#8221;* to the opening <xsl:stylesheet> tag, then, at least if you are using Xalan and Java to do the transformation, it will preserve the spaces between the tags in the XSL and you&#8217;ll get something more or less readable.</xsl:stylesheet><!--more-->

But then I tried using *xsl:attribute* to add attributes and I got the following error:  
`<br />
SystemId Unknown; Line #5; Column #101; Cannot add attribute x after child nodes or before an element is produced.  Attribute will be ignored.<br />
`

The XSL I was using was of the form:

<pre class="brush:xml">&lt;point&gt;
&lt;xsl:attribute name="x"&gt;&lt;xsl:value-of select="@period"/&gt;&lt;/xsl:attribute&gt;
&lt;/point&gt;</pre>

Without xml:space=&#8221;preserve&#8221; it works fine, but with it, I get the error. If I add the xml:space=&#8221;preserve&#8221; but put the xsl:attribute directly next to the point element (so<point><xsl:attribute xml:space="preserve"></xsl:attribute></point>There are a some work arounds:</p> 

  * Don&#8217;t put any space after the element and any xsl:attribute elements, you can put other tags (xsl:if for example), just not space.
  * Don&#8217;t use xml:space=&#8221;preserve&#8221;! You can still generate human readable XML by using *xsl:text*. Any space inside the xsl:text is preserved, including carriage-return/line-feed. So you can use that to format your XML&#8211;in fact you can do it more precisely, though perhaps at the risk of making your XSL more difficult to read.
  * Finally, I noticed that if you use *xsl:include* to include other xsl files, then you can have xml:preserve=&#8221;space&#8221; in some but not others. So, you might be able to get away with not including it in the sheets where you need to add the attributes.

Good luck