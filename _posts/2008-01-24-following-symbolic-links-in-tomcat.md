---
title: Following symbolic links in Tomcat
author: denish
layout: post
permalink: /2008/01/following-symbolic-links-in-tomcat/
categories:
  - Blog Entries
  - Java
  - Web development
tags:
  - Tomcat
  - Web development
---
We have several web applications derived from the same code base and as well as sharing the jars in WEB-INF/lib, we provide set of administration pages to allow users to configure and administer the applications. Until now, we&#8217;ve always had to make copies of the admin pages and this has caused us configuration headaches as we try and make sure that all the applications have the latest versions of all the pages.

But now I&#8217;ve finally managed to work out how to make Tomcat follow symbolic links—and it&#8217;s very easy!<!--more--> All you have to do is add 

`allowLinking="true"` to the Context tag in the context.xml file. For Tomcat 5.5 and later, I find the most convenient way is to have a META-INF directory in the web application directory and have a context.xml file in this. Like this all the files to do with the web application are in the same place and copying and deploying is easy. So, my context.xml looks like:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Context path="/myapp" allowLinking="true">
</Context>
```

As soon as you restart Tomcat, the change comes into effect and you can share directories between applications or even link to directories outside the web application home directory. If you still using Tomcat 4.x then you can&#8217;t use the context.xml in the META-INF directory. Instead you have to put it in the server.xml itself.
