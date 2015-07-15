---
title: Internationalising JSPs
author: denish
layout: post
permalink: /2006/08/internationalising-jsps/
categories:
  - Blog Entries
  - Java
  - Persistence
tags:
  - Java
  - Localisation
  - MySQL
  - UTF8
---
We recently had what sounded like a simple job to do: produce a questionnaire in several languages including Russian and store the results in a MySQL database. Now I could have chosen PHP to produce the questionnaire, but I thought that using Java resource bundles would be the easiest. I knew that using the JSTL fmt: tags we could do fmt:message and pull the messages out of a resouce bundle so all I had to do was get the translators to take the english property file, translate it and hey presto! Java knows about Unicode right into its core, so it would all work wouldn&#8217;t it.

**How wrong I was!**<!--more--> It turned out to be much more complicated than I thought. First I couldn&#8217;t easily get the Cyrillic text into the property files (Textpad which has been my staple text editor for as long as I can remember doesn&#8217;t support other character encodings, so that was a problem. IntelliJ seemed to work sometimes, but not others, the fmt:message didn&#8217;t find the properties, and then when I tried to save stuff to the database or even reflect back what users had typed into a text field I got gobbledigook.

To cut a long story short. Here&#8217;s what I needed to do:

1. Add <%@ page contentType=&#8221;text/html;charset=utf-8&#8243; %> to the top of each page and then <meta http-equiv=&#8221;Content-Type&#8221; content=&#8221;text/html; charset=utf-8&#8243;> into the section. This tells the browser that the text is coming in utf-8. It seems to affect how text from a form is posted back to the server. (Though in the end I didn&#8217;t need this to display the text as I&#8217;ll explain below.)

2. When saving text files on Windows, there are actually different encodings. I&#8217;d never noticed this before. Textpad can&#8217;t cope with them. IntelliJ can, but just saves it as it finds it. So if you have a normal text file and paste Cyrillic characters in and then save it, you just get a bunch of question marks. However, good old Notepad knows about this and when you do Save As, there&#8217;s a drop down on the File Save dialog that allows you to change the encoding to Unicode, or UTF-8. Unfortunately, **Java can&#8217;t cope with UTF-8 property files**! I just assumed I&#8217;d got it wrong somehow, but if you look at [http://bugs.sun.com/bugdatabase/view\_bug.do?bug\_id=6204853][1] you&#8217;ll see that it&#8217;s &#8220;in progress&#8221;.

3. So the trick I used (and it&#8217;s probably really naff&#8211;but works) is to use HTML Entities. The easiest way I found of converting these was to use Microsoft FrontPage 2003 in split design/code view. Take the property file with the Cyrillic text and paste it into the design view (in a <pre> section so you don&#8217;t get any HTML formatting), then copy back out the text in the code view. This translates the Russian for Yes (Да) to Да. You can convert all the properties in one go this way, so it&#8217;s not too much of a pain. You can then save the property file in the normal encoding and everything is fine. At the top of the JSP you add:

<pre class="brush:xml">&lt;fmt:setLocale value="${param.lang}"/&gt;
&lt;fmt:bundle basename="qtext"&gt;</pre>

And then put all of your messages in your-app/WEB-INF/classes called qtest\_ru\_RU.properties and so on for each language. You can then use <fmt:message key=&#8221;a.message.key&#8221;> to get the appropriate text.

4. Having got the display working, the next thing was to write the answers to MySQL. There are two parts to this, first you need to ensure that the database is expecting UTF-8. You can do this by setting the CHARSET of the table to UTF8, so something like:

<pre class="brush:sql">CREATE TABLE answers (
    ... column definitions
) <strong>CHARSET = utf8</strong>;</pre>

Or you can set individual columns to utf8 by doing something like ALTER TABLE answers MODIFY surname varchar(50) **CHARACTER SET utf8**;

That&#8217;s the first bit. To insert the text into the columns in the right format, I used the sql: tags in the JSTL, but you need to tell the SQL that it&#8217;s UTF8, and you do that by adding _utf8 before the string. So something like

<pre class="brush:javascript">&lt;sql:update sql="UPDATE answers SET surname = <strong>_utf8</strong>'${param.surname}'"&gt;</pre>

5. Finally, even though that works directly with the stuff posted back from a form, if you try just embedding ${param.surname} into your JSP page, you&#8217;ll get junk characters back. It seems this is because the encoding isn&#8217;t really UTF8 at all but ISO-8859-1. So to convert the request parameters back to UTF-8, you have to do something like:

<pre class="brush:javascript">&lt;%
    pageContext.setAttribute("surname",
       new String(request.getParameter("surname").getBytes("ISO-8859-1"), "UTF-8"));
%&gt;</pre>

This sets the JSTL variable &#8220;surname&#8221; to the UTF8 version of the surname parameter. And if you then use ${surname} it will display properly (but remember to save ${param.surname} and not ${surname} into the database!!

Hope this helps. Please let us know if you know of neater ways to do all this!

 [1]: http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6204853