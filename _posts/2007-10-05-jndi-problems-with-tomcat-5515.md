---
title: JNDI problems with Tomcat 5.5.15
author: denish
layout: post
permalink: /2007/10/jndi-problems-with-tomcat-5515/
categories:
  - Blog Entries
  - Java
---
I&#8217;ve recently been having difficulty getting JNDI lookup for MySQL connection pools working on our public server.

This proved very difficult to track down as all the stuff that I could find on the web said that I had to have:

  * commons-dbcp-1.2.1.jar
  * commons-pool-1.2.jar
  * mysql-connector-java-3.1.6-bin.jar

(or the latest appropriate versions)

Unfortunately this didn&#8217;t work and my little test JSP (using the JSTL sql tags) just gave me the error: DataSource invalid:  
&#8220;java.sql.SQLException: No suitable driver.

However, next I tried connecting to MySQL directly using `Class.forName(""com.mysql.jdbc.Driver")` and that worked. So the problem wasn&#8217;t in loading the driver.<!--more-->

Trying the JNDI steps manually in a Java scriptlet gave a different error. Here&#8217;s the code:

<pre>Context initContext = new InitialContext();
out.println("1. Got the initial context");
String envContextName = "java:/comp/env";
Context envContext  = (Context)initContext.lookup(envContextName);
out.println("2. Got the context: "+envContextName);
String jndiName = "jdbc/myTest;
DataSource ds = (DataSource)envContext.lookup(jndiName);
out.println("3. Got the datasource: "+ds.getClass().getName()+" for context "+jndiName);
Connection conn = ds.getConnection();
out.println("4. Got the connection: "+conn);</pre>

This time the error I got was: `javax.naming.NamingException: Cannot create resource instance`. So it seemed as if some class was missing, but it wasn&#8217;t obvious which one.

I had the whole thing working fine on my development server, so I decided to see which classes were being loaded and where from. I found that you can find where a class has been loaded from quite easily (see [here][1]).  
So I created another test JSP with the following Java snippet (you&#8217;ll have to add the appropriate imports):

<pre>&lt;%
// A list of classes to try and load and see where they come from
String[] classNames = {
"com.mysql.jdbc.Driver",
"org.apache.commons.dbcp.datasources.SharedPoolDataSource",
"org.apache.tomcat.dbcp.dbcp.BasicDataSource",
"org.apache.tomcat.dbcp.dbcp.PoolableConnection"
};

for (int i = 0; i &lt; classNames.length; i++) {
String className = classNames[i];
out.print("

"+className+"
");
try {
// try and load the class
Class c = Class.forName(className);
out.println("loaded successfully:  "+c+"");
// if successful, find out the location (gives the location of the jar)
ProtectionDomain pDomain = c.getProtectionDomain();
CodeSource cSource = pDomain.getCodeSource();
out.println("    Location:  "+cSource.getLocation()+"");
} catch (ClassNotFoundException e) {
out.println("<strong>Error could not find class</strong>");
} catch (Throwable t) {
out.println("<strong>Error could not load</strong> ("+t+")");
}
out.println("

");
}
%&gt;</pre>

I chose these classes to test because when running the initial test on my local machine, I found that the DataSource class created was `org.apache.tomcat.dbcp.dbcp.BasicDataSource`. Running this on my development machine told me that the last two classes were in the jar `naming-factory-dbcp.jar` which mysteriously wasn&#8217;t in common/lib on our production server. Copying it there and restarting Tomcat did the trick and now the JNDI datasource look up works fine.

So, that&#8217;s another jar you need to make sure is in your Tomcat common/lib directory if you&#8217;re having JNDI lookup problems, it&#8217;s also a general technique that can be used to find out where classes are loaded from and which classes are available on the class path and which are missing.

 [1]: http://www.jugpadova.it/articles/2005/11/13/from-which-jar-a-class-was-loaded