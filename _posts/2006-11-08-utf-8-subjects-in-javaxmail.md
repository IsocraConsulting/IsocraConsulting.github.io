---
title: UTF-8 subjects in javax.mail
author: denish
layout: post
permalink: /2006/11/utf-8-subjects-in-javaxmail/
categories:
  - Blog Entries
  - Java
tags:
  - Internationalisation
  - Java
  - Mail
  - UTF8
---
I&#8217;ve been having problems with putting unicode strings into the subject of emails sent using javax.mail.

In the end, the solution was very simple and I found it here:

<http://www.velocityreviews.com/forums/t132009-utf8-characters-not-appearing-correctly-in-email-subject-line.html>

As long as you use &#8220;UTF-8&#8243; and not &#8220;UTF8&#8243; it all seems to work fine, so the code you need is:

<pre class="brush:java">Properties props = new Properties();
<em>// put in your SMTP host in here</em>
props.put("mail.smtp.host", "localhost");
Session s = Session.getInstance(props, null);

MimeMessage message = new MimeMessage(s);
message.setFrom(new InternetAddress(from));
message.addRecipient(Message.RecipientType.TO, new InternetAddress(to));
message.setSubject(subject, "UTF-8");
message.setText(body, "UTF-8");
message.setHeader("Content-Type", "text/plain; charset=UTF-8");
Transport.send(message);</pre>