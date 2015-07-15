---
title: MacBook, MissingSync and Vodafone 3G USB stick
author: denish
layout: post
permalink: /2009/01/macbook-missingsync-and-vodafone-3g-usb-stick/
categories:
  - Blog Entries
tags:
  - MacBook
  - Mobile Broadband
  - Windows Mobile
---
I&#8217;ve been using a MacBook for a couple of years now and really like. However, one of the downsides is that I can&#8217;t simply sync with my HTC TyTn II mobile running Windows Mobile 6 in the same way that you can with a PC running Windows.  
<!--more-->

  
There are a couple of apps out there that bridge the gap and the one I&#8217;ve been using for most of that time is [MissingSync][1]. This works pretty well and uses the Mac SyncServices to synchronise with iCal, Mail, Entourage, Folders, [Yojimbo][2], music, photos, videos, installing apps and setting the time. So all the good stuff that you&#8217;d like it to do.

It&#8217;s not as seemless as using ActiveSync, but it works pretty well (I regularly get iCal/MissingSync conflicts and errors in syncing which I rarely got with ActiveSync on the PC, but they&#8217;re all manageable).

Then suddenly a month ago or so, I couldn&#8217;t sync via USB any more. I tried upgrading to the latest version of MissingSync and contacted their technical support and they suggested switching off various features such as Advanced Networking on the phone, but none of this worked. I could sync via bluetooth (but bluetooth on my MacBook disappears sporadically&mbar;due to old age??), but the USB connection wouldn&#8217;t work. 

Normally when syncing, you get a transparent panel that appears on the MacBook screen and then the first of four blobs appears. When the second one appears, there&#8217;s a message to log in to the phone, so you type in your password (assuming you&#8217;ve set one) and then blobs three and four appear, then it syncs.

In my case, the first blob appeared and then it timed out. Looking at the MissingSync log, I&#8217;d see something like:

<pre>1/9/09 19:21:39  Monitor:  Connection status from 0 to 135
1/9/09 19:21:41  Monitor:  Connection status from 135 to 135
1/9/09 19:22:12  Monitor:  setUICallback
1/9/09 19:22:12  MSUI:  setMonCallback
1/9/09 19:22:23  Monitor:  Connection Failed.  Telling MSUI
1/9/09 19:22:23  Monitor:  makeSureUIAppIsRunning
1/9/09 19:22:23  MSUI:  Connection Failed for USB RNDIS
1/9/09 19:23:09  Monitor:  Listener: closeCurrentDeviceConnection 1, state=1
1/9/09 19:23:09  Monitor:  forgetDevice  rraServer:0
1/9/09 19:23:09  Monitor:  Connection status from 0 to 0
1/9/09 19:23:09  MSUI:  disconnect
</pre>

To cut a long story short, I eventually worked out that the important thing I had changed was installing the software for my new [Vodafone mobile broadband][3] USB modem. This installs a new network service that you can see in the network settings (mine is called Vodafone K3520). I&#8217;ve also got (in addition to the basic ones) two installed by [Parallels Desktop][4] (Parallels Shared Networking Adapter and Parallels Host-Only Networking Adapter). These don&#8217;t affect syncing with Missing Sync.

<img src="/images/VodafoneK3520NetworkService.png" alt="Disabling the Vodafone network service" style="float: left; padding-right: 8px" />As a test, I removed the Vodafone K3520 from the network preferences and then tried to sync and it worked fine. I synced several times before I needed to use my mobile broadband modem again. To get that working again, I just reinstalled the software from the USB stick. After using the broadband several times during the day, I tried syncing again&mbar;no luck! So I went back to the Network Preferences, selected the Vodafone K3520 and selected &#8220;make service inactive&#8221; (you then have to press the Apply button too to make it effective). I then tried syncing again and it worked fine. 

So now I can use my mobile broadband and sync my TyTn II albeit by disabling and re-enabling the Vodafone network service.

**Update:** Just to be sure of my facts for this blog posting, I downloaded and installed the latest version of [Vodafone Mobile Connect][5] (version 2.11.02.00 at the time of writing). It installed another couple of network services called DIAG and PCUI). I don&#8217;t quite understand what&#8217;s going on here because I&#8217;m sure I had these originally and then when I reinstalled from my Vodafone stick I only got the K3520 mentioned above. Now I have 3! But when I connect to mobile broadband, only the K3520 is enabled, DIAG and PCUI say not connected. 

But the really good news is that now, I no longer need to disable the K3520 service. I&#8217;ve just synced again without incident and am finishing this post using the mobile broadband. (I couldn&#8217;t do both at the same time because the stick is too wide to allow a second USB device to be plugged in along side.)

 [1]: http://www.markspace.com/products/windowsmobile/mac/windows-mobile-sync-software.html
 [2]: http://www.barebones.com/products/Yojimbo/
 [3]: http://shop.vodafone.co.uk/shop/mobile-broadband/data-devices/
 [4]: http://www.parallels.com/
 [5]: http://www.business.vodafone.com/site/bus/public/enuk/support/10_productsupport/laptop_connectivity/40_software/software/10_latest/p_software.jsp