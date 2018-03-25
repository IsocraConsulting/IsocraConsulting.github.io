---
title: Accessing ADSI from Java
author: denish
layout: post
permalink: /2005/05/adsi-from-java/
categories:
  - Articles
  - Java
tags:
  - ADSI
  - Java
  - JNI
  - LDAP
  - security
---
The aim of this project was to be able to get the groups that an NT user belongs to using ADSI (Active Directory Services Interface) and make this available in Java. In the [first article][1] we described how to get ADSI information using VBScript, JScript and then C++. Now we show how to wrap up the C++ function in a DLL that&#8217;s accessible using JNI.<!--more-->

## Java Native Interface (JNI)

This article isn&#8217;t a tutorial on JNI in general, there are much better resources for that (for example [ Sun&#8217;s JNI Tutorial][2]). All we need to understand in order to access ADSI are the following steps (assuming that we&#8217;re using C++ to develop the native code):

  * We first define a Java class with a static method which has the signature that we want to have in Java. Java provides a special `native` keyword to designate such a method.
  * Java provides a tool `javah` which will generate a C++ header file. This header file contains the C++ signature equivalent to the Java static method. It also includes `jni.h` which is provided with the JDK.
  * We need to rewrite our C++ ADSI program as a DLL and implement the method declared in the generated header file.

In addition to this JNI provides C++ classes that allow you to create and convert between Java types and C++ types. These are accessed via the `env` parameter which is the extra first parameter to the C++ function (and which doesn&#8217;t appear in the Java declaration). The `env` object is an instance of `JNIEnv`.

## Writing the Java

This is the easiest bit. We simply create a new class (in our case `com.isocra.utils.javaADSI.ADSIUtil`) and add a static method:

```java
public class ADSIUtil {

    /** returns an array of strings containing the groups for the current user */
    public static native String[] groupsForUser(String user);

    // ...
}
```

Note the `native` keyword, this tells the compiler that the method is going to be actually implemented in an external DLL. We also need to tell the Java which DLL to load. The DLL can be loaded statically or dynamically. Here we&#8217;ll go for the easiest option and load it statically. We do it by calling `System,loadLibrary("userGroups")`. The easiest place to put this is in the `ADSIUtil` class, so we&#8217;ll add:

```java
/** static library loading code - loads the external C++ DLL */
static {
    System.loadLibrary("userGroups");
}
```

This means that the DLL is loaded when the class is loaded at runtime. In order to find the DLL, Java looks in the current directory and in `java.library.path`. So if you&#8217;re not going to put the DLL in the current directory, you&#8217;ll need to add `-Djava.library.path=<em><path to DLL></em>` to the command line.

<p class="note">
  Note the class might not be loaded exactly when you expect, it will depend on the class loader. With Tomcat for example, the class loader used will depend on whether you put your class in the <code>WEB-INF/lib</code> directory of your application, or in the <code>common/lib</code> directory. With Tomcat also, the current directory is the <code>bin</code> directory, so that&#8217;s the easiest place to put the DLL. If you want to put it elsewhere, then you will need to add <code>-Djava.library.path=&lt;em>&lt;path to DLL&gt;&lt;/em></code> to the <code>TOMCAT_OPTS</code> environment variable (the exact way you set the <code>java.library.path</code> changes with the different versions of Tomcat.
</p>

You call the method just as you would any static method, so our test program has the following method:

```java
/** Simply take the username from the command line, call the ADSI
 * routine to get the list of users and write them out to System.out
 */
public static void main(String[] args) {
    if (args.length != 1) {
        System.out.println("Usage: testJavaADSI &lt;username&gt;");
    } else {
        String[] groups = ADSIUtil.groupsForUser(args[0]);
        if (groups != null) {
            System.out.println("testJavaADSI: "+args[0]+" belongs to " +
                    groups.length+" groups:");
            for (int i = 0; i &lt; groups.length; i++) {
                System.out.println("["+i+"] "+groups[i]);
            }
        } else {
            System.out.println("testJavaADSI: no groups returned");
        }
    }
}
```

## Writing the C++ DLL

Our previous article on [ADSI from C++][3] explains the fundaments behind accessing ADSI. The difference here is that we need to implement the JNI method and make sure that it returns an array of Java strings.

To generate the C++ header file for `ADSIUtil.groupsForUser()`, we first call `javah`. In fact, we used an [ANT task][4] to do it repeatably and easily as follows:

```xml
<target name="makeDLLHeader"
        description="<em>build the header file from the class file</em>">
    <javah
        classpath="${build.dir}"
        verbose="true"
        destdir="${cpp.src.dir}"
        >
        <class name="com.isocra.utils.javaADSI.ADSIUtil"/>
    </javah>
</target>
```

This generates a header file called `com_isocra_utils_javaADSI_ADSIUtil.h` where the package hierarchy and the classname are all encoded into the filename. The header file contains an include for `jni.h` some standard C++ stuff to ensure the contents aren&#8217;t executed more than once and the following declaration. (Remember the Java declaration is: `<strong>public static native</strong> String[] groupsForUser(String user)` ).

```
JNIEXPORT jobjectArray JNICALL Java_com_isocra_utils_javaADSI_ADSIUtil_groupsForUser
  (JNIEnv *, jclass, jstring);
```

You can immediately see that `String[]` has been mapped to a type called `jobjectArray`, and there are now three formal parameters instead of just one: `JNIEnv *`, `jclass`, and `jstring` which is the actual parameter we pass in from the Java. The first parameter is a pointer to the environment which gives access to all sorts of useful functionality such as creating java objects, the second is a pointer to the calling class which can be useful for accessing features on that class and the last parameter is ours.

To implement the function, we create a C++ file called, say, `userGroups.cpp` and include the header file as well as any other header files we&#8217;d normally need. We define the function with actual rather than formal parameters:

```
JNIEXPORT jobjectArray JNICALL Java_com_isocra_utils_javaADSI_ADSIUtil_groupsForUser
  (JNIEnv *env, jclass caller, jstring jUsername)
{ ...
```

In order to be able to use `jUsername`, we to access the characters in the array, we can do this by calling `env->GetStringUTFChars(jUsername, 0)` which returns a `char *`. We can then use this just like we did in the C++ example (except that where we just did `return`, we need to do `return NULL` now).

The problem comes when we want to return the list of groups associated with the specified user. To do this, we need to use the environment to allocate us an object array as follows: `env->NewObjectArray(size,env->FindClass("java/lang/String"),NULL);` Note though that we need the size of the array. As far as we can tell, there isn&#8217;t a way to query to see how many groups a user is in (if someone knows a way to do this then please let us know at <feedback@isocra.com>). So, we have to iterate round the groups counting them, then iterate round again to copy them into the newly created array.

We can do this easily enough as follows, once we have the enumeration containing the groups. Basically, we go and get the next element in the enumeration one at a time until we can&#8217;t get any more or the `HResult` is not `S_OK`:

```
int size = 0;
hr = pEnum-&gt;Next(1, &var, &lFetch);
while(hr == S_OK && lFetch == 1)
{
    size++;
    <em>// Now go and get the next group</em>
    hr = pEnum-&gt;Next(1, &var, &lFetch);
}
<em>// Now we know the size, we reset the enumeration</em>
pEnum-&gt;Reset();
```

Once we have the size, we reset the enumeration so that we can go round again and then we can allocate the Java String array:

```
// With the size, we can now create the object array we need
jobjectArray jarray = env-&gt;NewObjectArray(size,
                                          env-&gt;FindClass("java/lang/String"),
                                          NULL);
```

The three parameters to `NewObjectArray()` are the number of elements (which we&#8217;ve just counted); the class of element we will be adding (notice that we have to call `env->FindClass()` and that we specify the class using `"java/lang/String"` rather than `"java.lang.String"`); and the initial value of the elements of the array (in this case `NULL`).

Now we can go though the enumeration again and get each element as a group so that we can get it&#8217;s name. Again this is just like we did in the plain C++ example (we could instead fetch all the elements in the enumeration with `pEnum->Next(size, &var, &lFetch)` but there doesn&#8217;t seem much point since we&#8217;d have to iterate through them anyway to convert them and put them into the Java array). Once we have a group name, we can use `env->SetObjectArrayElement(<em>array</em>, <em>index</em>, <em>string</em>)`. The first two parameters are easy, but the third needs a little more work as we currently have the name of the group as an array of wide chars and we need a Java String. We can allocate a Java String using `env->NewStringUTF()` but this still takes a `char *` rather than what we have. Microsoft&#8217;s ATL and MFC libraries have utility functions for converting from one to the other, but since we are using the Borland C++ compiler we don&#8217;t have access to these. However, the conversion isn&#8217;t hard, and we have included a simple implementation of `W2CA` (Wide char to ANSI) which does the job by providing a nice wrapper to `WideCharToMultiByte()`:

```
char* W2CA(const WCHAR* wideStr) {
   int wideLen = 255; // wstrlen(wideStr);
   char* asciStr = new char[wideLen+2];

   WideCharToMultiByte(
      CP_ACP,
      WC_COMPOSITECHECK,
      (LPCWSTR)wideStr,
      -1,
      asciStr,
      wideLen+1,
      NULL,
      NULL
   );

   return asciStr;
}
```

Now we can copy the group name into the Java array as follows:

```
env-&gt;SetObjectArrayElement(jarray, i++, env-&gt;NewStringUTF(W2CA(bstr)));
```

Once we&#8217;ve looped through all the elements, we can release the enumeration, call `CoUninitialize()` and then return the allocated in filled in Java array.

Now that we&#8217;ve written the C++, we can compile it as a DLL. (To find information on using the excellent free Borland C++ compiler, [click here][5]).

At the end of this article, the resources section allows you to download our test application and sample code including an ANT script to build both the Java and the C++.

## End note

Unfortunately, you will find when you run the little test program (and the same goes for the C++ version), that it takes a noticable time—probably too long to be used as a filter on web-page access which is what we were originally hoping to use it for.

## <a title="resources" name="resources"></a>Resources

  * [Download the source][6] – all the java and CPP source for the project plus an ANT script to build both the Java and C++ (assuming the Borland C++ compiler)
  * [View the JavaDoc][7].
  * [Apache&#8217;s ANT build tool][8]
[ – a must-have tool if you are developing in Java ][8]

  * [Instructions on using the (free) Borland C++ compiler][9] to compile the C++ DLL.

 [1]: 2005/04/adsi-from-cpp
 [2]: http://java.sun.com/developer/onlineTraining/Programming/JDCBook/jni.html
 [3]: adsi_from_cpp.php
 [4]: http://ant.apache.org/ "Apache ANT build tool"
 [5]: bcc.php
 [6]: /articles/javaADSI.zip
 [7]: /articles/javaADSI/index.html
 [8]: http://ant.apache.org/
 [9]: /2005/03/free-borland-cpp/
