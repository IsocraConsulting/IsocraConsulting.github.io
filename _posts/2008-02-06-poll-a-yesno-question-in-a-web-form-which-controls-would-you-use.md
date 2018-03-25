---
title: 'Poll: A yes/no question in a web form, which control(s) would you use?'
author: denish
layout: post
permalink: /2008/02/poll-a-yesno-question-in-a-web-form-which-controls-would-you-use/
categories:
  - Blog Entries
  - Web development
tags:
  - design
  - HTML
  - usability
  - Web
  - Web development
---
We&#8217;re creating a web application that asks a number of questions about pumps (as it happens). Some of these questions are typical yes/no questions. But we need to make sure the user makes a positive choice. We don&#8217;t want to impose a choice on them. Which controls would you use?

Here&#8217;s the question with the three options we&#8217;ve currently got:

<table class="table table-striped">
  <tr>
    <td>
      Do you need a sealless pump?
    </td>
    
    <td>
      <select name="option1"><option>[Please Select]</option><option>Yes</option><option>No</option></select>
    </td>
  </tr>
  
  <tr>
    <td>
      Do you need a sealless pump?
    </td>
    
    <td>
      <input name="option2" value="YES" id="option2Yes" accesskey="y" type="radio" /> <label for="option2Yes">Yes</label><br /> <input name="option2" value="NO" id="option2No" accesskey="n" type="radio" /> <label for="option2No">No</label>
    </td>
  </tr>
  
  <tr>
    <td>
      Do you need a sealless pump?
    </td>
    
    <td>
      <input name="option3" value="YES" id="option3" type="checkbox" /><label for="option3">Yes</label>
    </td>
  </tr>
</table>

Jakob Nielsen at useit.com in his article [Checkboxes vs Radio Buttons][1] tells us that radio buttons are used &#8220;when there is a list of two or more options that are mutually exclusive and the user must select exactly one choice&#8221; and a single checkbox is used &#8220;for a single option that the user can turn on or off&#8221;. But this case is a little special since we don&#8217;t want the user to forget to select something. The nice thing about using the radio buttons or the drop-down is that you have a way of detecting if the user has actually chosen anything yet. On the whole though, the drop-down list is probably the least user-friendly though perhaps the easiest to generate programmatically.

**This poll is now closed.**

 [1]: http://www.useit.com/alertbox/20040927.html
 [2]: http://lesterchan.net/wordpress/
