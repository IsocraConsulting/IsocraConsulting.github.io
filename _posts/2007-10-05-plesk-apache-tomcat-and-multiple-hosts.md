---
title: Plesk, Apache, Tomcat and multiple hosts
author: denish
layout: post
permalink: /2007/10/plesk-apache-tomcat-and-multiple-hosts/
categories:
  - Blog Entries
  - Web development
---
In addition to having problems with JNDI not working properly, I&#8217;ve also been bashing my head against Plesk&#8211;that is supposedly &#8220;designed to simplify the management and administration of web sites&#8221;.

It&#8217;s the interaction between Plesk and Tomcat that&#8217;s the particular problem and how both of them negotiate dealing with multiple domains (or rather don&#8217;t deal with it).<!--more-->

Plesk allows you to have multiple domains hosted on the same machine. So though the domains have the same IP address, you get different web pages, different email addresses etc. Tomcat also allows you to configure multiple hosts in its server.xml file.

Our set up had Tomcat having two services: Catalina and PSA. Catalina ran on 8080 as usual and had one host&#8211;localhost&#8211;and PSA ran on 9080 and had hosts for each of the domains set up in Plesk.

From inside Plesk, you can enable and disable the different Tomcat hosts. You can also upload WAR files for new web apps (which it stores in different webapps directories as set up in the server.xml). In fact I believe that Plesk uses the Tomcat manager API to load up the web apps etc&#8211;though I don&#8217;t know this for certain.

The problem came with mapping these uploaded web applications through Apache so that they appear as normal directories and web pages.

This is normally set up using JKMount, something of the general form, is added to the httpd.conf Apache configuration file:

```apacheconfig
<ifmodule>
JkMount /myapp ajp13
JkMount /myapp/* ajp13
</ifmodule>
```
This says pass anything starting with /myapp to the ajp13 worker. The ajp13 worker is defined in workers.properties (normally in the same directory as the httpd.conf file). Generally the default version of workers.properties is fine enough.

But in this case, the ajp13 worker is set up to route things through to the normal Tomcat set up, i.e. port 8009 and host name &#8220;localhost&#8221;. So that means that any web apps that you want to run on host xxx actually have to be installed in localhost and are also available as localhost:8080. Of course, that&#8217;s not where Plesk has put them. Plesk has installed them in the PSA service for the host that you configured, so not surprisingly it doesn&#8217;t work. So&#8230;

**Manual work around no.1:**

Don&#8217;t bother using Plesk to upload your webapps. Instead just upload them yourself using FTP directly into the normal webapps directory. You can then test that they work using your main URL and port 8080. You can also use the Tomcat manager app to upload and to start and stop as necessary.

Once you&#8217;re happy that you&#8217;ve got them working you can wire them into Apache as follows:

1. First, don&#8217;t edit the httpd.conf file directly. Instead go to the &#8216;Plesk&#8217; configuration directory for your particular host, so if you have www.mydomain.com, then the chances are you&#8217;ll find this in `/var/www/vhosts/mydomain.com/conf` or something like that.  
2. In there you should find an `httpd.include` file. Don&#8217;t edit that either!! (because it will get overwritten by Plesk)  
3. Instead, create a new file called `vhost.conf`. This is included by the httpd.include file (if you look through it you&#8217;ll see the directive).  
4. In the `vhost.conf`, put the normal JKMount directives to your webapp using the format shown above. This will direct anyone looking for that directory on that particular host to the webapp you&#8217;ve installed in the normal Tomcat webapps (for the localhost host).  
5. The easiest way to get Apache to notice the new configuration is to restart it, generally you can do this by logging in to your server using ssh or whatever and typing `/sbin/service httpd restart`. You should see some reassuring messages about restarting and all should be well!

This will give you what you need and allow you the control you need. However, it does mean that all your web applications are sitting together in the same webapps directory, and they&#8217;re all accessible using the port 8080. So that brings us to&#8230;

**Manual work around no. 2**

Again, I&#8217;m not sure there&#8217;s any point in using Plesk to upload your web applications because it will put the wrong things in the `httpd.include` file and once a web app is there, if you fiddle with it, you can get the Plesk configuration out of sync with what&#8217;s actually in the file system and I haven&#8217;t worked out how to fix that yet (so I have web apps in Plesk that I&#8217;ve deleted manually, and I can&#8217;t get rid of them in Plesk).

This time, though upload them into the &#8220;right&#8221; place. Look in Tomcat&#8217;s server.xml to find out where the appbase is for your particular host.

Next comes the &#8220;brave&#8221; bit. You&#8217;ll need to create a new worker in workers.properties. If you look at the workers.properties file, you&#8217;ll see that the ajp13 worker is defined something like the following:

```apacheconfig
worker.ajp13.port=8009
worker.ajp13.host=localhost
worker.ajp13.type=ajp13
#
# Specifies the load balance factor when used with
# a load balancing worker.
# Note:
#  ----> lbfactor must be > 0
#  ----> Low lbfactor means less work done by the worker.
worker.ajp13.lbfactor=1
```

So, it specifies that port 8009 is used for AJP and the host is localhost. That&#8217;s why using ajp13 sent your requests to localhost!

We want to add a new worker that uses our host&#8217;s port and hostname. Looking at your server.xml again, if you have another service (PSA as we do), you&#8217;ll find that there&#8217;s an AJP/1.3 protocol connector, say on port 9009. And let&#8217;s assume that your host is called mydomain.com as before.

So you need to create a new worker as follows:

```apacheconfig
worker.mydomain.port=9009
worker.mydomain.host=mydomain.com
worker.mydomain.type=ajp13
# I'm not sure if this is needed for load balancing, but it
# doesn't seem to do any harm if you add it
worker.mydomain.lbfactor=1
```

I think you also need to add it to two other places. At the top of the file you&#8217;ll find the worker.list:

```apacheconfig
worker.list=ajp12, ajp13, jboss, mydomain
```

And nearer the bottom, I also added it to the load balancer:

```apacheconfig
worker.loadbalancer.balanced_workers=ajp12, ajp13, jboss, mydomain
```
The next step is to create/edit the `vhost.conf` file as described above, but instead of using ajp13, use your new worker `mydomain` for example:

```apacheconfig
JkMount /myapp   mydomain
JkMount /myapp/* mydomain
```

Now restart Apache as before and you should find that any requests for the URL /myapp on your specified host should now route through to the right application in the right Tomcat host.
