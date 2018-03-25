---
title: Using the Free Borland C++ compiler
author: denish
layout: post
permalink: /2005/03/free-borland-cpp/
categories:
  - Articles
  - C++
tags:
  - C++
  - free tools
---
If you have access to Visual C++, then compiling C++ to produce an EXE or a DLL isn&#8217;t particularly hard. If not, then you either have to use `gcc` or there is an excellent free C++ compiler provided by Borland. You can download `freecommandlinetools.exe`<!--more--> from the [Borland FTP site][1].

To install, simply run the EXE and decide where you want to install it. To use it on the command line, you&#8217;ll need to add `%INSTALL-DIR%BorlandBCC55Bin` to your path (we have a little batch file that does this for us).

Since you&#8217;re likely to want the compiler and linker to include various standard header files and libraries, you can set up a configuration file to do this for you:

  1. Go to `%INSTALL-DIR%BorlandBCC55Bin` and create a `bcc32.cfg` file.
  2. To add include directories, you need to specify `-I"<directory>;<directory>..."` and to add library directories, you need to specify `-L"<directory>;<directory>..."`. For our ADSI examples, we need both the normal include directories and the aditional PSDK library subdirectory. Since we had installed the compiler into `C:toolsBorland` our config file looks like this: <code class="brush:text">-I"C:toolsBorlandBcc55include"
-L"C:toolsBorlandBcc55lib;C:toolsBorlandBcc55LibPSDK"</code>

To compile a simple EXE (such as the [ADSI test example][2]) all you need to do is  
call:

```bcc32 -tWM &lt;yourfile&gt;.cpp```

The `-tWM` tells the linker that we want a 32-bit multi-threaded windows application. For the ADSI example,  
we need to add the `activeds.lib` library to include the ADSI functions. So the command becomes:

```bcc32 -tWM activeds.lib getUserGroups.cpp```

When building for JNI, we need to add the Java header files. We can either do this in the `bcc32.cfg` file (by adding the  
absolute path the the JDK include directories), or (if `JAVA_HOME` is set), you can add them to the command line as follows.

```bcc32 -tWM -tWD -I%JAVA_HOME%/include -I%JAVA_HOME%/include/win32 &lt;yourfile&gt;.cpp```

Here, as well as adding the includes, we&#8217;ve added `-tWD`, this tells the linker that we want to make a DLL.

One of the things that I like about using the Borland compiler is that it&#8217;s straight-forward to integrate it into an ANT  
build script. So, our `build.xml` has the following targets to build our JNI DLL:

```xml
<target name="makeDLL"
    description="build the header file from the class file, then compile the DLL">

    <!-- first (re-)make the header file that we include in our C++ file -->
    <javah
        classpath="${builddir}"
        verbose="true"
        destdir="${cppdir}"
        >
        <class name="com.isocra.util.ADSIGetUserGroups"/>
    </javah>

    <!-- now execute the borland C++ compiler.
         NB I had to play around with the env key to make sure it set the
         path correctly (in particular the case of the key).
         I'm not convinced this will work in all cases -->

    <exec dir="${cppdir}" executable="${cppCompilerDir}bcc32.exe">
        <arg line="-tWM -tWD -I${java.include} -I${java.include}/win32 dllGetUserGroups.cpp"/>
        <env key="Path" path="${cppCompilerDir}"/>
    </exec>

</target>
```

 [1]: ftp://ftpd.borland.com/download/bcppbuilder/freecommandLinetools.exe
 [2]: 2005/04/adsi-from-cpp/
