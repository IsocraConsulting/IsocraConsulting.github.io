---
title: UTF-8 with Hibernate 3.0 and MySQL
author: denish
layout: post
permalink: /2007/01/utf-8-with-hibernate-30-and-mysql/
categories:
  - Blog Entries
  - Java
  - Persistence
tags:
  - Hibernate
  - Java
  - MySQL
  - Persistence
---
Hi again, I&#8217;m now tackling something I&#8217;ve been meaning to do for ages (and probably should have done before I started the project) and that is to use Hibernate rather than rolling my own SQL.

I&#8217;m reading the book [&#8220;Hibernate in Action&#8221;][1] by Christian Bauer and Gavin King and have been trying it out. The book relates to Hibernate 2.0 and 2.1 so doesn&#8217;t include annotations which is a bit of a shame, but it&#8217;s still a good start.

Unfortunately as readers of this blog will know, I&#8217;m trying to put UTF-8 strings into the database so you can imagine my disappointment when I saw the familiar question marks appearing where there should be interesting new characters.

The fix turned out to be not so difficult to find however, thanks to Philip Whirlycott&#8217;s blog posting [More UTF-8 head-thumping with Hibernate 3][2] (obviously I&#8217;m not the only one struggling with these issues).

With Hibernate 3 (maybe not with 2.1, I&#8217;m not sure), you need to add some extra connection parameters:

jdbc:mysql://localhost/mydb?autoReconnect=true&useUnicode=true&characterEncoding=UTF-8

If you put this in your hibernate.cfg.xml file for the JDBC URL it works and you can save the UTF-8 correctly.

 [1]: http://www.amazon.co.uk/Hibernate-Action-Practical-Relational-Mapping/dp/193239415X/
 [2]: http://www.whirlycott.com/phil/2006/05/28/more-utf-8-head-thumping-with-hibernate-3/