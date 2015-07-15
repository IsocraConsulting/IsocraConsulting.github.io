---
title: Converting Lisp universal times to Java Dates
author: denish
layout: post
permalink: /2006/03/converting-lisp-universal-times-to-java-dates/
categories:
  - Blog Entries
  - Java
tags:
  - Java
  - Lisp
---
I don&#8217;t suppose that this is something that many people need to do, but just in case I thought I&#8217;d post this little code snippet<!--more-->:

<pre class="brush:java">public class TestLispUniversalTime {

    /** the number of milliseconds between 1990 and 1970 */
    private static final long msecsTo1970 = -2208988799684L;

    public static Date convertToDate(long lispDate) {
        return new Date((lispDate * 1000)+msecsTo1970);
    }

    public static void displayDate(Date thisDate) {
        System.out.println(thisDate.getTime()+" is "+thisDate);
    }

    /**
     * Java Dates are stored in milliseconds since 1970. Lisp
     * dates are stored in seconds since 1900, so to convert between the two
     * we need to take away the number of seconds between 1 Jan 1970 and 1 Jan 1900
     * and then multiply by 1000 to get milliseconds (probably).
     * @param args
     */
    public static void main(String[] args) {
        // First find out how many milliseconds between 1970 and 1900
        Calendar calendar = Calendar.getInstance();
        calendar.set(1900, 0, 1, 0, 0, 0);
        System.out.println("This is 1st Jan 1900");
        displayDate(calendar.getTime());
        // To convert between
        System.out.println("Lisp thinks this is 17:45:25 28/4/2003");
        displayDate(convertToDate(3260537125L));
        System.out.println("This is what Java thinks is now:");
        displayDate(new Date());
        System.out.println("Lisp thinks this is 00:00:01 on January 1, 1976 GMT");
        displayDate(convertToDate(2398291201L));
        System.out.println("Lisp thinks this is 00:00:01 on January 1, 1900 GMT");
        displayDate(convertToDate(1L));
    }
}</pre>