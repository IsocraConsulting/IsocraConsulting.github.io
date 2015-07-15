---
title: Just what is software architecture?
author: Tim Hoverd
layout: post
permalink: /2008/01/just-what-is-software-architecture/
categories:
  - Architecture
  - Blog Entries
tags:
  - Architecture
  - design
  - requirements
---
So often we hear people saying they&#8217;re &#8220;architecting&#8221; some piece of software, or that they&#8217;re a high-powered architect of some form? I was musing what that should mean. At least one colleague insists that it&#8217;s just a pompous word for design, optimised for use in proposal boilerplate when you&#8217;re trying to aggrandise your role in life.

I&#8217;m not so sure, but there clearly is something called software design, and it&#8217;s really closely related to software architecture if that&#8217;s a separate issue.

After some thought, I&#8217;ve decided (I&#8217;ll probably change my mind soon) that software *design* is about designing those parts of a system that address the system&#8217;s *functional requirements*. *Architecture*, on the other hand, is about designing a system so that it meets that system&#8217;s *non-functional requirements*.

So, for example, if I&#8217;m designing something so that orders are properly sent to the billing engine, or so that the aircraft&#8217;s elevators move to the correct angle then I reckon that&#8217;s design. If I&#8217;m adding a write-back cache to an enterprise system so that it works effectively with 2000 simultaneous users when it used to support 5, then I reckon that&#8217;s architecture.

Or am I just still being pompous?