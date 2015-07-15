---
title: Getting NT groups in Java using Jacob
author: denish
layout: post
permalink: /2006/05/getting-nt-groups-in-java-using-jacob/
categories:
  - Blog Entries
---
I&#8217;m running Tomcat on a windows box and I want to use Windows/Domain users and groups to control access to web application pages. So far I&#8217;ve thought of a number of different approaches, but none of them are quite right yet.<!--more-->

The first one I thought of was to write a new Tomcat Realm. This would mean that people would have to log in again with their normal username and password, but after that all the web applications could use the normal security mechanisms and these would be validated against the Windows Domain groups (which the Realm would have instantiated on log in).

The problem with this is that it&#8217;s not actually that easy to validate a username and password in Windows. Presumably for (actually quite obvious) security reasons, there isn&#8217;t a straight-forward API that lets you say &#8220;does this username and password work?&#8221;. So I abandonned that approach.

The next approach (and the one I&#8217;m stuck with at the moment), was to use a filter. The filter can mimic the NTLM challenge-response mechanism (if you&#8217;ve got IE clients) and get the username (and password!!) from the client machine. (You can look at some sample code here: [//www.rgagnon.com/javadetails/java-0441.html][1], it also has links to other sites with more detail. In the example, they have the code in a JSP, but it&#8217;s better to have it in a filter which can be applied to all pages.)

So having got the username, I now need to get the groups the user is in. The best current way is to use ADSI, and there&#8217;s an article I wrote on how to do this from scratch [here][2]. But recently I&#8217;ve been using [Jacob][3] which is a Java to COM bridge and makes everything much easier. Here&#8217;s the code needed to get the groups when using Jacob:

<pre class="brush:java">public static List GetUserGroups(String domain, String username) {
        ActiveXComponent user = new ActiveXComponent("WinNT://"+domain+"/"+username);

        ArrayList groupsList = new ArrayList();

        // now get the groups
        Variant[] params = new Variant[]{};
        Variant groups = user.invoke("Groups", params);
        Dispatch groupsDispatch = groups.toDispatch();
        EnumVariant groupsEnum = new EnumVariant(groupsDispatch);
        while (groupsEnum.hasMoreElements()) {
            Variant group = groupsEnum.Next();
            Dispatch pGroup = group.toDispatch();
            Variant groupName = Dispatch.get(pGroup, "Name");
            groupsList.add(groupName.toString());
        }
        return groupList;
    }</pre>

In order to use the code in your web apps, you need to bundle not only the Jacob JAR but also the DLL and then you need to put the java.library.path into the TomCat OPTS so that it can find the DLL.

It&#8217;s best not to put these in your web app&#8217;s WEB-INF/lib directory because then you won&#8217;t be able to reload the application (you&#8217;ll get errors saying that the native library has already been loaded in another context). So instead, put it in the common/lib directory and everything should be fine.

 [1]: http://www.rgagnon.com/javadetails/java-0441.html
 [2]: http://isocra.com/articles/adsi_from_java.php
 [3]: http://sourceforge.net/projects/jacob-project/