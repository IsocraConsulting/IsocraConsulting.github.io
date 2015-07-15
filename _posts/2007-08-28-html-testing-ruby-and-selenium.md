---
title: HTML Testing, Ruby and Selenium
author: denish
layout: post
permalink: /2007/08/html-testing-ruby-and-selenium/
categories:
  - Blog Entries
  - Web development
tags:
  - Testing
  - Web development
---
One of the problems with using HTML as the user interface is testing it (in fact testing user interfaces has always been a bit of a problem) so I&#8217;m always on the look out for good UI testing tools.

For a while I&#8217;ve known about and used [Watir][1] which scripts Internet Explorer (and now Firefox with [FireWatir][2] though it&#8217;s a bit more tricky to set up). Watir allows you to write simple but powerful scripts in Ruby that will instruct IE to load up a page, fill in a form, click a button or whatever and then you can make assertions about what should have happened. It&#8217;s not difficult to use and even if you don&#8217;t know Ruby, if you know other languages, you can soon pick it up.

Today however I came across [Selenium][3] (also on the same OpenQA site). This is neat for two reasons: (1) it&#8217;s all written in JavaScript so the same script runs in both IE and Firefox, on the PC or Mac; and (2) it has an IDE (a Firefox extension in fact) which lets you record the tests rather than having to write them yourself (you can then save them away and create test suites; you can also load up tests and single step them, etc.). (There is a Watir recorder on the OpenQA site but it&#8217;s only version 0.1 so far. There&#8217;s also [WET][4] which I haven&#8217;t tested, but is a Windows application written in Ruby which wraps around Watir.)

The thing I&#8217;m currently looking for though is a test environment where I don&#8217;t have to install a large infrastructure and which works easily on both PCs and Macs, supporting both IE and Firefox. Selenium seems to give me exactly that. In fact the project&#8217;s customer could probably install it on their PC and be able to run the tests (or even record new tests) which would be great.

Please let me know if you know of other tools that would fit the bill or what your experience is with these tools.

 [1]: http://www.openqa.org/watir/
 [2]: http://wiki.openqa.org/display/WTR/FireWatir+-+WATiR+for+FireFox
 [3]: http://www.openqa.org/selenium/
 [4]: http://wet.qantom.org/