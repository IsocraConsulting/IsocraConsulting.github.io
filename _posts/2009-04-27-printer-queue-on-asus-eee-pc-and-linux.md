---
title: Printer Queue on Asus EEE PC and Linux
author: denish
layout: post
permalink: /2009/04/printer-queue-on-asus-eee-pc-and-linux/
categories:
  - Blog Entries
tags:
  - CUPS
  - EEE PC
  - printing
---
My son has an Asus EEE PC and it&#8217;s great for what he needs. He can easily carry it around and it has all the power he needs for doing his homework.

The only issue we&#8217;ve had is that our HP2605DN network printer keeps getting assigned a different IP address when we occasionally restart the network and this messes up printing from the Asus. Generally I only find out when he&#8217;s pressed print several times, so what I&#8217;d like to do is go to the print queue, remove all the pending jobs and then correct the IP address.

But the weird thing is that I couldn&#8217;t find the printer queue anywhere (we use JetDirect network printing). I searched the internet and Asus forums and just couldn&#8217;t find how to do it. The Macs on the network just find the printer using Bonjour and aren&#8217;t put out by the IP address change at all.

Finally, I discovered how to find out all about the printers and queues on the Asus EEE, you simply point your web browser at [http://localhost:631][1]. Both the Asus (running Linux) and Macs use [CUPS][2] (Common Unix Printing System&trade;). CUPS maintains a web server on the client machine on port 631, so all you have to do is go to the web server and then you can use the web pages to see the printers and jobs, add new printers, configure printers, cancel jobs, everything you&#8217;d like to be able to do. This works on the Asus EEE, and on Macs too.

 [1]: http://localhost:631/
 [2]: www.cups.org
