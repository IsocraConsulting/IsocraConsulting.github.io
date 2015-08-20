---
title: Making Xerces ignore a DTD
author: denish
layout: post
permalink: /2006/05/making-xerces-ignore-a-dtd/
categories:
  - Blog Entries
---
I&#8217;m just trying to write a SAX parser for the XML log files generated from the Logging API if you use the XMLFormatter.

There are a couple of problems with the XML generated. One is that the XML is not well formed until the log file is &#8220;completed&#8221; because it doesn&#8217;t have a final </log> element (not surprising really).<!--more-->

The second is that it declares a DTD as follows:

{% highlight xml %}
<?xml version="1.0" encoding="windows-1252" standalone="no"?>
<!DOCTYPE log SYSTEM "logger.dtd">
{% endhighlight %} 

Now it&#8217;s fairly easy to find out what the DTD should be, it&#8217;s in the JavaDoc and a quick search on Google reveals it. But I can&#8217;t guarantee it&#8217;ll be in the right place so that I can just load it.

No problem, I thought, I&#8217;ll just turn off validation and then it won&#8217;t matter. Wrong! Even with validation switched off, it still tries to load it and I get a FileNotFoundException.

However, by delving in the source code I finally found that if you switch off two features, then it does ignore the external DTD. So add this to your code if you want to ignore DTDs:

{% highlight java %}
final SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();

final SAXParser saxParser = saxParserFactory.newSAXParser();
final XMLReader parser = saxParser.getXMLReader();

// Ignore the DTD declaration
parser.setFeature("http://apache.org/xml/features/nonvalidating/load-external-dtd", false);
parser.setFeature("http://xml.org/sax/features/validation", false);
{% endhighlight %}
    

If anyone knows a better way then please let me know!
