---
title: Image buttons made simple
author: denish
layout: post
permalink: /2008/06/image-buttons-made-simple/
categories:
  - Blog Entries
  - jQuery
  - Web development
tags:
  - CSS
  - HTML
style: |
  button.imageButton {
    border: 1px solid silver;
    background-color: white;
    background-repeat: no-repeat;
    background-position: 1px center;
    padding: 1px 1px 1px 28px;
    height: 30px;
    cursor: pointer;
    font-family: Arial;
  }
  button.add {
    background-image: url(/images/add.gif);
  }
  button.remove {
    background-image: url(/images/remove.gif);
  }
---
I read a blog post recently showing how to create a button with an image and text. The button looked very simple and very nice to look at, but the HTML was more complicated than I thought it needed to be.

There is a move now to make the HTML of a document as simple and self-explanatory as possible. The HTML should contain the *intent* of the page, CSS (and possibly JavaScript) is then used &#8220;skin&#8221; the page as required. This has become more feasible now that browsers are finally maturing and supporting more and more of the CSS standard.

So, if we want a button, we should use the `<button>&#8230;</button>` tag. If we want to have an add button, we want to have:

{% highlight html %}
<button class="imageButton add"
       onclick="addSomething(); return false;">Add Something</button>
{% endhighlight %}

And instead of this

<button onclick="alert('Hello!'); return false;">Add Something</button>

we want it to look like this:

<button class="imageButton add" onclick="alert('Hello!'); return false;">Add Something</button>
<!--more-->

This seems to convey everything that we need without adding any extra HTML. There is also a move towards unobtrusive JavaScript and if we use jQuery, then we could put the onclick call into the $(document).ready(&#8230;) function and that would make the HTML simpler (on the other hand it would mean a dislocation between the functionality and the button and would mean that developers looking at the HTML would have to search around to find out where the functionality was specified.

## Now to the CSS

I&#8217;m allowing myself two classes, &#8220;imageButton&#8221; and &#8220;add&#8221; I think this is justified because the first tells me that I want this button to look like an image button and the second one tells me it is an add button.

{% highlight css %}
button.imageButton {
    border: 1px solid silver;
    background-color: white;
    background-repeat: no-repeat;
    background-position: 1px center;
    padding: 1px 1px 1px 28px;
    height: 30px;
    cursor: pointer;
    font-family: Arial;
}
{% endhighlight %}

The imageButton class changes the overall look of any button with this class. It changes the border to a single pixel solid silver. You can change this to any colour you like or even remove the border completely. It also sets the background colour and then sets up some defaults for the image for the button. We want the image to be on the left (indented by 2 pixels so it isn&#8217;t touching the border), but in the centre vertically. We also want to make sure that the text is indented so that there is room for the image (I&#8217;m assuming that I know that all image are 24px square). For this reason I also set the button height to be 30px to allow some space above and below the image. Finally it sets the cursor to a pointer so that it&#8217;s obvious when the user moves over it that it&#8217;s a button and sets the font to something appropriate. This is what a button with this style looks like. We can add the imageButton class to all our buttons to make them look the same.

<button class="imageButton" onclick="alert('Hello!'); return false;">Basic</button>

Now we need to add the image. I&#8217;m adding this as a background image so that it doesn&#8217;t interfere with the HTML and doesn&#8217;t appear as selectable to the user. All we need is a very simple  
selector for the add class:

{% highlight css %}
button.add {
    background-image: url(/images/add.gif);
}
{% endhighlight %}

This adds the image and there we go. Now all the add buttons throughout the website can reuse the same two classes and adding other buttons (for example a remove button) is as simple as  
adding another image to the CSS:

{% highlight css %}
button.remove {
    background-image: url(/images/remove.gif);
}
{% endhighlight %}

<button class="imageButton add" onclick="alert('Hello!'); return false;">Add Something</button>&nbsp;<button class="imageButton remove" onclick="alert('Goodbye!'); return false;">Remove Something</button>

Now there&#8217;s one final flourish that we can add. We can add a *:hover* class so that when the user moves over the button they get some visual feedback. This is pretty straightforward  
to add. All we need is:

{% highlight css %}
button.imageBtn:hover {
    text-decoration: underline;
    border: 1px solid gray;
}
{% endhighlight %}

This changes the border to a darker grey and underlines the text. Here&#8217;s the final result:

<button class="imageButton add" onclick="alert('Hello!'); return false;">Add Something</button>&nbsp;<button class="imageButton remove" onclick="alert('Goodbye!'); return false;">Remove Something</button>

Unfortunately on IE6 you don&#8217;t get the hover effect and on Firefox 2.0.0.14 you get the border change but not the text underlining. Also, in IE6 and IE7, the buttons are much wider than I&#8217;d  
like them to be. It isn&#8217;t the border or the padding because I set those explicitly, it isn&#8217;t the margin either because I&#8217;ve tried setting that. You could set the width explicitly, but of course  
that means that you might get the text not fitting into the button. If anyone knows another fix, please let me know.
