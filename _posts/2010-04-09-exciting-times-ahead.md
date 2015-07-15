---
title: Exciting times ahead
author: Tim Hoverd
layout: post
permalink: /2010/04/exciting-times-ahead/
categories:
  - Architecture
  - Concurrency
  - Hardware
---
I&#8217;ve been musing about the future, so forgive me for a bit of crystal ball gazing.

For years we&#8217;ve been building complex software systems, confidence that the underlying hardware was getting quicker and quicker courtesy of <a href="http://en.wikipedia.org/wiki/Moore's_law" target="_self">Moore&#8217;s law</a>. This still boggles me; I stopped being a professional electron herder many years ago and at the time we were talking about the feature size on silicon dice getting down to the heady depths of 1 micron. There&#8217;s now people talking blithely about features in the region of a tenth of this size and this week there&#8217;s an announcement from HP that they&#8217;ve finally  managed to fabricate the famed <a href="http://en.wikipedia.org/wiki/Memrister" target="_self">memristors</a>, or at least one type of them, on silicon offering the hope of even higher densities. (Although, to be fair, memristors are not transistors which is what Gordon Moore was actually talking about.)  
<!--more-->

  
Up to now we&#8217;ve managed to just get faster and faster processors so we&#8217;ve just been able keep blundering on with our old approach to software. In 1971 there were about 2,000 transistors on a single processor die, and now there are about 2 billion. That&#8217;s a million-fold improvement in the raw performance of the silicon. Could we honestly say that the software we write is a million times more effective than it was in 1971? I doubt it. What&#8217;s more, I think there&#8217;s a real problem coming up.

Although the chip makers are still following the Moore&#8217;s law yellow brick road it&#8217;s clear that in recent years they haven&#8217;t been able to just make faster and faster processors as they&#8217;re too close to the limits of the underlying physics. Rather, they&#8217;re building multicore architectures which allow a single processor die to genuinely do many things in parallel. Just about all of us have dual core machines on our desks and 4 and 8 core server chips are common. Most software design has been able to ignore this issue up to now because we&#8217;ve just been able to allow allow separate threads to be scheduled to one core or another and there have been enough threads to use the processors fully. In particular in server farms each of a number of transactions has been able to be scheduled to a core at a time.

However, many of us will have been aware of times when we&#8217;re running some CPU intensive activity like video processing and the little CPU load indicator says that exactly 50% of our super-duper dual core chip is being used. That&#8217;s because some bozo designed that video processing software to be single threaded and as such it can only use a single core at a time. As time goes by that software will be able to use less and less of our machines. Moore&#8217;s law dictates that in 18 months we&#8217;ll all be buying 4 core machines, in 3 years we&#8217;ll be getting 8 core machines and in 6 years time we&#8217;ll routinely see 32 core machines. This is really happening, this year Intel demonstrated a prototype of a 48-core chip, each core being a roughly PC-like machine.

So, in 6 years time that video processing software will be able to use 1/32th of a typical processor which won&#8217;t seem that wonderful.

How are we going to use those cores? It seems to me that we&#8217;re finally going to have to learn to write properly concurrent programs. Many of us will have flirted with this over the years, especially with special concurrent programming languages like <a href="http://en.wikipedia.org/wiki/Occam_programming_language" target="_self">occam</a>, <a href="http://en.wikipedia.org/wiki/Erlang_(programming_language)" target="_self">Erlang</a> and <a href="http://en.wikipedia.org/wiki/X10_(programming_language)" target="_self">X10</a>. In many case we&#8217;ll remember the extreme annoyance with writing software that just deadlocked or livelocked all over the place and how difficult it was to understand what was going on. As such we&#8217;ve all retreated back to a nice serial (or pseudo-serial via a transactional mechanism) world. It seems to me that we&#8217;re going to have to start getting back to working out how to use fine-grained concurrency. It&#8217;s going to be really quite difficult and, I suspect, a pretty bumpy ride.

Sounds like fun&#8230;