---
title: Groovy, DSLs and Rules engines
author: denish
layout: post
permalink: /2008/01/groovy-dsls-and-rules-engines/
categories:
  - Articles
  - Groovy
  - Java
  - Web development
tags:
  - Groovy
  - Java
  - Rules
  - Web
---
This article describes how we used <a href="http://groovy.codehaus.org/" target="_blank">Groovy</a> to create a <span style="border-bottom: 1px dotted red" title="Domain Specific Language">DSL</span> for the clauses in a rules engine. We have been collaborating in the development of an &#8220;expert system&#8221; application for <a href="http://www.knowledgebench.com" title="www.knowledgebench.com" target="_blank">KnowledgeBench</a> Ltd. This application uses a forward chaining rules engine to provide a &#8220;smart&#8221; system capable of creating formulations in domains such as pharmaceutical tablet formulation, inhalers, paints, food & drink and cosmetics. <!--more-->

When we first started developing the system, we had fairly specific requirements for what the rule language should be able to do and this meant that off-the-shelf packages such as [Ilog&#8217;s JRules][1] and [Drools][2] weren&#8217;t applicable.

The KnowledgeBench architecture has three parts:

  * A *knowledgebase* which is a collection of objects with user-definable properties organised in a hierarchy and supporting inheritance.
  * The* rules engine* which has access to the knowledgebase and to user&#8217;s current session, including any information submitted from HTML forms
  * A set of *web pages* which display information and provide the user with ways of interacting with the system.

So, for the rules, we effectively needed a DSL which would make it easy to reason about the available information and carry out actions. Furthermore, we knew from experience with similar systems, that we needed the ability to do pattern matching in the rules, effectively trying different bindings for variables within the rules to find which ones matched.

After searching and trying various expression languages we settled upon <a href="http://groovy.codehaus.org/" target="_blank">Groovy</a> which provided us with the richness we needed as well as the ability to embed it in our execution environment (it now provides lots more and we&#8217;re evaluating using Groovy Server Pages and <a href="http://grails.codehaus.org/" target="_blank">Grails</a>).

Let&#8217;s take a specific example. For many formulation applications, the structure is as follows: there are a number of categories of ingredient that need to be added and for each category, one or more ingredients will need to be chosen. A &#8220;specification&#8221; of requirements will determine which ingredient categories need to be added, and within that, specific ingredients will be selected to provide a formulation that meets the requirements.

Our architecture is as follows. Each rule belongs to a ruleset. Each rule has a number of condition *clauses* and if these are &#8220;true&#8221; then the rule is said to fire and the action clauses are executed. In KnowledgeBench, the rules are Java objects, but the clauses are Groovy expressions. The rules engine sets up the initial environment by binding a set of variables (such as the knowledgebase and an object representing the current &#8220;state&#8221; including request parameters and any properties that previous rules might have set up). This environment is then passed to Groovy and the expression is evaluated.

We have two types of clause, one which simply evaluates to true or false and one that binds a new (local) variable. The first type is straight forward, but the second is interesting because we support a clause returning a list of possible bindings. So for example if we have an object in the knowledgebase who&#8217;s ID is &#8220;FLOUR&#8221; and which has a number of instances (actual types of flour) each with properties, we might have a clause as follows:

<pre><strong>IF</strong>
flour := kbase.findById('FLOUR').instances
...
</pre>

Here, we&#8217;re trying to create new bindings for the &#8220;flour&#8221; variable. &#8220;kbase&#8221; is aways in scope and refers to the knowledgebase itself. The rest of the clause looks up the object whose ID is &#8220;FLOUR&#8221; in the knowledgebase and returns all the instances. This is a list, so there are multiple possible bindings for the &#8220;flour&#8221; variable. Each of these bindings is carried forward one after the other to the next clause to see if it can be true. If a binding works, then it&#8217;s carried to the next clause and so on. If all the clauses can be made to be true with a particular binding, then that binding is carried through to the actions and each action is executed. The variable itself is local to the rule, so changes to it do not automatically affect the environment. But action clauses can set properties in the environment, create new objects or whatever.

Types of flour might have a &#8220;grade&#8221; property. For making bread, we need &#8220;OOO&#8221; grade (say), so a rule to get all the appropriate types of flour might look as follows:

<pre><strong>IF</strong>
flour := kbase.findById('FLOUR').instance
flour.grade == 'OOO'
<strong>THEN</strong>
params.addValue('flourCandidates', flour)
</pre>

Here the next clause checks to see if the grade is &#8220;OOO&#8221;. &#8220;flour.grade&#8221; uses Groovy&#8217;s ability to intercept property accessors and look for a method called &#8220;getGrade()&#8221;, when it doesn&#8217;t find that on the (Java) object, it then looks for a get(String property) method. We&#8217;ve implemented this to access the attributes in our (KnowledgeBench) objects in the knowledgebase. This means that accessing properties is obvious and straight forward in the DSL.

The action clause uses another built-in variable &#8220;params&#8221; which holds the current session. In knowledge-based systems speak it&#8217;s the &#8220;blackboard&#8221;. Rules can read and write information from/to it and any request parameters from the user interface (such as the result of posting a form) are automatically made available as properties.

Why does Groovy work so well? I think there are several reasons for this:

  * We needed a language that would allow methods to be called with parameters, not just expressions, so expression languages such as JEXL weren&#8217;t appropriate.
  * We wanted succinct syntax, so being able to access properties by simply using the name (as you can do in JEXL and in the JSP EL&#8211;though you can&#8217;t do this for our &#8220;soft&#8221; properties or attributes since these are defined at run-time, not as Java classes) is very useful. Also the pseudo maps work very intuitively.
  * Our developers are also writing JSP pages, so the similarity between Groovy and the JSP EL means that it&#8217;s relatively easy to switch between the two. In fact we&#8217;ve developed our own Groovy eval tag that takes a Groovy expression and evaluates it in the the same context as the JSP but making the knowledgebase etc. available just like in a rule clause.
  * We needed good integration between the language and our Java code. Groovy is great from this point of view, all our classes and methods are available in an easy to use and obvious way.
  * We are able to use the extra methods that Groovy provides on collections such as *findAll* and *inject* to add even more power to our rule language.
  * We use Groovy closures in some cases to do advanced calculations, for example one of the things we often need to do is add up all the current ingredient percentages to see how much we need to add. This can be done in a single clause: `formulation.attributes.inject(0) {total, attr -> total += attr.allValues[0] }` this returns the total having added up all the attribute values of the object refered to by &#8220;formulation&#8221;.

The combination of these aspects, together with the fact that it&#8217;s now a Java standard and has lots of support mean that we&#8217;re very pleased with out choice.

 [1]: http://www.ilog.com/products/jrules/
 [2]: http://labs.jboss.com/drools/