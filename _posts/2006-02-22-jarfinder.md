---
title: 'Which jar contains my&#8230;'
author: denish
layout: post
permalink: /2006/02/jarfinder/
categories:
  - Articles
  - Java
tags:
  - Java
---
Have you ever had the problem where there&#8217;s a class missing and you&#8217;ve no idea which jar it might be in? Or been running a Java program and wondered why the implementation of a class doesn&#8217;t seem to match what you think, and it turns out that there&#8217;s another implementation earlier in the classpath? Or wondered which jar has the properties file you&#8217;re looking for?

If you&#8217;ve answered yes to any of these question, you might like these little utilities<!--more-->:

  * **DirectorySearcher** will look recursively through a directory tree, search all files and jars for all entries that match the regular expression you&#8217;ve specified.
  * **ClasspathSearcher** will search your classpath (individual files, directories and inside jars) for  
    all entries that match your search criterion.

Both of these utilities use the same core visitor pattern to search through and match files (even inside jars). There&#8217;s an abstract `EntryVisitor` class that does most of the work and currently there&#8217;s only one concrete implementation of that—`EntryNameVisitor`— but it would be easy to add others that, for example, found classes created in a certain data range.

## How it works

The key class in this framework is the `EntryVisitor`. It&#8217;s an abstract class which implements the classic Visitor pattern. So it, and any subclasses, have a method `visit(...)` which is called for every entry in the search space. What visit actually does is up to the specific subclass.

In fact our `EntryVisitor` has two (abstract) visit methods, one which takes both a `JarFile` and a `JarEntry` and one which takes a `File`. This is because we want to be able to match against both entries inside jars and files in directories and unfortunately there isn&#8217;t a common class we can use. In fact there&#8217;s a second reason too: if we find a match within a jar, we want to output the name of the jar that we found it in too.

In order to get the results of the visits back, we have a very simplistic `getResults()` method that just returns a string. In the case of our `EntryNameVisitor`, this simply contains a list of all the matching filenames (including paths so that you can find them again) or in the case of jar entries, the name and path of the jar followed by the matching entry. All this is followed by the number of entries found (which is useful if you&#8217;re tweaking the classpath for example and want to see quickly if it makes a difference). If there are no entries found, then it says so and echoes back the pattern it was looking for.

`EntryVisitor` has 3 other (concrete) methods:

`public boolean <strong>isJar</strong>(java.io.File file)`
:   This simply checks the extension of the specified file to see if it is .JAR (regardless of case). It&#8217;s used internally when searching through directories to detect jar files, but it&#8217;s also available externally (and is used by the `ClasspathSearcher` to know waht to do with a particular element).

`public void <strong>searchJar</strong>(java.io.File jarFile)`
:   This searches through the entries in the specified jar and calls visit on every entry.

`public void <strong>searchDir</strong>(java.io.File directory)`
:   This searches through the specified directory, if it finds a jar it calls `searchJar`, if it finds a subdirectory, it calls `searchDir` recursively, for everything else, it calls `visit(File file)` so that we can match against individual files too.

`EntryNameVisitor` simply implements the two versions of `visit` to do the regular expression matching on the name of the file or JarEntry. The constructor takes a pattern string and then calls Pattern.compile(patternStr) to get a pattern (see [java.util.regex.Pattern][1]) so note that this utility will only work with JDK 1.4 and above.

In the visit methods, we create a matcher using `Pattern.matcher()` and then we can just call `matcher.matches()` to see if there&#8217;s a match. So, for the file version, the code is:

{% highlight java %}
public void visit(File file) {
    // Create a matcher
    Matcher matcher = _pattern.matcher(file.getName());
    // See if it matches
    if (matcher.matches()) {
        // Output the absolute path so that we know where the file is
        _output.append(file.getAbsolutePath()+"n");
        // Increment the count so we know how many we have found
        _count++;
    }
}
{% endhighlight %}


## Wrapping it up

Now that we have the core, we need to wrap it in something we can actually use. We&#8217;ve implemented two wrappers, one searches a specified directory and one searches the CLASSPATH.

Directory searching is trivial because we already have a method for searching a directory on `EntryVisitor`, so all we have to do is check we&#8217;ve got the right number of arguments, check we&#8217;ve got a directory, create a new `EntryNameVisitor` passing it the pattern string and then telling it to search the specified directory. Once it&#8217;s finished, we call `getResults()` and write that to `System.out`.

CLASSPATH searching is only marginally more complicated. We can get at the CLASSPATH by calling `System.getProperty("java.class.path")` and we can get the classpath separator by calling `System.getProperty("path.separator")`. Once we have this, we can use the `StringTokenizer` to get each element. We then have to check whether an element is a jar (and call `searchJar()`), whether it is a directory (and call `searchDir()`) otherwise, just call `visit(File file)` on it.

Arguably a more elegant approach would be to implement an enumeration which could make both directories and  
the CLASSPATH look the same so that we wouldn&#8217;t need to have essentially the same code in two places, but given that it&#8217;s pretty short and it is in only two places (so far), we&#8217;ll let ourselves off!

 [1]: http://java.sun.com/j2se/1.4.2/docs/api/java/util/regex/Pattern.html
 [2]: /articles/jarSearchSrc.zip
 [3]: /articles/jarfinder/index.html
