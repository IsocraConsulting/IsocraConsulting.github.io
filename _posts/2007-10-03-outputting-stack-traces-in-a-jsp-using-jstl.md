---
title: Outputting stack traces in a JSP using JSTL
author: denish
layout: post
permalink: /2007/10/outputting-stack-traces-in-a-jsp-using-jstl/
categories:
  - Blog Entries
  - Java
  - Web development
tags:
  - Java
  - JSP
  - Web development
---
If you&#8217;re writing a JSP using the JSTL tag <c:catch var=&#8221;myError&#8221;> &#8230; </c:catch> then I&#8217;m sure that you know that the next thing to do is to check after the closing catch tag to see if myError is empty. If not you can output some useful error message having successfully caught any exception. However, sometimes it&#8217;s useful to actually output the stack trace (when debugging a site for example). It turns out not to be difficult to do this. Simply copy in the following:


```jsp
<c:if test="${not empty myError}">
<p class="error">An error occured: <c:out value="${myError}"/></p>
<pre>
<c:forEach var="stackTraceElem" items="${myError.stackTrace}">
<c:out value="${stackTraceElem}"/><br/>
</c:forEach>
</pre>
```
