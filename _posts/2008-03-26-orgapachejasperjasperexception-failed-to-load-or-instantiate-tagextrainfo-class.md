---
title: 'JasperException: Failed to load or instantiate TagExtraInfo'
author: denish
layout: post
permalink: /2008/03/orgapachejasperjasperexception-failed-to-load-or-instantiate-tagextrainfo-class/
categories:
  - Blog Entries
  - Java
tags:
  - Java
  - JSP
  - Web development
---
## org.apache.jasper.JasperException: Failed to load or instantiate TagExtraInfo class

(I had to shorten the article title to fit!) 

I&#8217;ve just had a problem when I&#8217;ve taken a web application from my local Tomcat 5.5 development server (where it worked fine) and uploaded it onto a public server (also using Tomcat 5.5). 

<!--more--> I copied the whole application including all the jars in the WEB-INF/lib directory. The web application started up fine and showed up in the Manager as running properly, but when I went to the home page (index.jsp), I got an error saying that there were problems reading the TLD (I am using 

[SiteMesh][1]) in one of the jars, and in particular that it couldn&#8217;t instantiate or load the TagExtraInfo class.

Disabling SiteMesh didn&#8217;t help, though I discovered it only affected JSPs not HTML pages or Groovy Server Pages.

Comparing the list of Jars in WEB-INF/lib with another application that worked, I discovered that I had included **jsp-api-2.0.jar**. Removing this jar and restarting the application from the manager sorted the problem.

I&#8217;m not sure why I didn&#8217;t get the same error on the development server but presumably the problem arises from version incompatibilities.

If you get similar problems, try making sure that you haven&#8217;t got this extra Jar in your WEB-INF/lib directory.

 [1]: http://www.opensymphony.com/sitemesh/