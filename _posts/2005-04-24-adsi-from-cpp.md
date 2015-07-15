---
title: 'Accessing Microsoft&#8217;s ADSI&#8211;part 1'
author: denish
layout: post
permalink: /2005/04/adsi-from-cpp/
categories:
  - Articles
  - C++
tags:
  - ADSI
  - C++
  - LDAP
  - security
---
The overall aim of this project was to be able to get the groups that an NT user belongs to using ADSI (Active Directory Services Interface) and make this available in Java. This first article describes how to get ADSI information using VBScript, JScript and then C++. It shows how the different languages make it easier or harder.<!--more--> In the 

[second article][1], we show how to wrap up the C++ function in a DLL that&#8217;s accessible using JNI.

## Accessing ADSI from VBScript and JScript

ADSI is a useful technology that provides easy access to users, groups, domains, etc. For more information on ADSI, have a look at the <a ref="http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnactdir/html/msdn_adsifaq.asp">MSDN site</a>. ADSI is available on Windows 2000 as well as Windows XP and doesn&#8217;t require domains or Active Directory. You can test out whether this will work on your machines by creating this small VBScript and then simply double clicking on it:

<pre class="brush:vb">rem listGroups.vbs
rem change the text below to have your computer and username
set user=GetObject("WinNT://DOLPHIN/denish")
for each group in user.groups
  msgbox group.name
next</pre>

The key is the [`GetObject`][2]  
function. This allows you to bind to directory service objects without you needing to provide any extra authentication. `GetObject` can be used to connect to ADSI (by specifying the `WinNT:` protocol) or LDAP (using `LDAP:`). `GetObject` is available to any language that supports Automation, so that includes VBScript, VB, JScript, C#, Delphi and other .NET languages. The sort of object that you get back will depend on what you query. You can query for users, groups, machines etc. In our case we want to find out which  
groups our user is in. Since when we were testing we didn&#8217;t have a domain, we used the machine name followed by the username. More normally, you would use the domain name followed by the username.

Given that JScript supports automation, it should be just as easy to write the code in JScript. However, VBScript is able to simply iterate through the groups using `for each`. In JScript we have to create an `Enumerator` first. We can initialise the Enumerator in the constructor like this: `var e = new Enumerator(user.Groups());`. There are then methods such as `atEnd()` and `moveNext()` that allow you to move through the enumeration.

Here is an ASP page that you can test using IIS which does slightly more than the script above (we used ASP because it was much easier to find bugs and output more complex results during development):

<pre class="brush:html">&lt;%@ language = JScript %&gt;
&lt;h1&gt;User information&lt;/h1&gt;
&lt;%
var user=GetObject("WinNT://DOLPHIN/denish");
if (user != null) {
%&gt;
&lt;table&gt;
&lt;tr&gt;&lt;td&gt;Class:&lt;/td&gt;&lt;td&gt;&lt;%=user.Class%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Name:&lt;/td&gt;&lt;td&gt;&lt;%=user.Name%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Full Name:&lt;/td&gt;&lt;td&gt;&lt;%=user.FullName%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Description:&lt;/td&gt;&lt;td&gt;&lt;%=user.Description%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Last Login:&lt;/td&gt;&lt;td&gt;&lt;%=user.LastLogin%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Password Expiration date:&lt;/td&gt;&lt;td&gt;&lt;%=user.PasswordExpirationDate%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Account Disabled:&lt;/td&gt;&lt;td&gt;&lt;%=user.AccountDisabled%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td valign="top"&gt;Groups:&lt;/td&gt;&lt;td&gt;
&lt;%
    for (var e = new Enumerator(user.Groups()); ! e.atEnd(); e.moveNext())
    {
        var oGroup = e.item();
        Response.write(oGroup.Name+"&lt;br&gt;n");
    }
%&gt;&lt;/td&gt;&lt;/tr&gt;
&lt;/table&gt;
&lt;%
} else {
    Response.write("User is null");
}
%&gt;
&lt;/body&gt;
&lt;/html&gt;</pre>

## Writing an ADSI test program in C++

`GetObject` is only available to languages supporting automation. Since we want to use JNI eventually, we&#8217;re really tied to using C++, and a fairly basic version of that so that the DLL is nice and portable with as few externak dependencies as possible. So, for C++ we have to use [`ADsGetObject`][3].  
This function is quite a bit more &#8220;basic&#8221; than `GetObject`. It takes the query string (which must be an array of WideChars, so we have to convert our string to WideChars first), then the ID of the type interface that we want back (in this case `IID_IADsUser`) then the address of a pointer to the object we want to get back as it will allocate the space and update our pointer. Finally it returns an HRESULT which we can examine to see if the call has been successful or not.

Since we want to eventually wrap this as a JNI DLL, we&#8217;re going to write this code to be more general than our examples above, so we won&#8217;t hard-code in the username, instead we&#8217;ll pass it in as a parameter to a function  
`void getGroupsFor(const char *user)`. This will then prepend the &#8220;WinNT://&#8221;, convert the string to a wide string, call `ADsGetObject`, check the result and assuming everything&#8217;s OK, get the groups and (for now) simply print them out to `stdout`. The function is shown below:

<pre class="brush:cpp">void getGroupsFor(const char *user)
{
    // First we have to make up the string that we're going to pass to ADsGetObject
    char adsPath[80];
    strcat(adsPath, "WinNT://");
    strcat(adsPath, user);
    fprintf(stdout, "adsPath is '%s'n", adsPath);

    // Call CoInitialize to initialise COM
    fprintf(stdout, "Starting up, calling CoInitialisen");
    CoInitialize(NULL);

    // Convert the adsPath to wide chars so that we can pass it to ADsGetObject
    int nLen = strlen(adsPath) + 1;
    WCHAR* widePath = new WCHAR [nLen];
    mbstowcs(widePath, adsPath, nLen);

    // Now set up the required variables and call ADSGetObject
    fprintf(stdout, "Now calling ADsGetObjectn");
    IADsUser *pUser;
    HRESULT hr = ADsGetObject(widePath,
                IID_IADsUser,
                (void**) &pUser );

    // If ADsGetObject can't return the user, we can stop now
    if(FAILED(hr)) {
        fprintf(stderr, "Got an error getting the user: %x", hr);
        return;
    }

    // Now let's get the groups
    IADsMembers *pGroups;
    hr = pUser-&gt;Groups(&pGroups);
    if(FAILED(hr)) {
        fprintf(stderr, "Got an error getting the groups: %x", hr);
        return;
    }
    // Don't need the user any more
    pUser-&gt;Release();

    // In order to iterate through the groups we need to get an enumerator
    // We then don't need the groups object itself any more so we can release it
    IUnknown *pUnk;
    hr = pGroups-&gt;get__NewEnum(&pUnk);
    if (FAILED(hr)) {
        fprintf(stderr, "Got an error getting the enumeration: %x", hr);
        return;
    }
    pGroups-&gt;Release();

    // From IUnknown interface, we can query to get the EnumVARIANT interface
    IEnumVARIANT *pEnum;
    hr = pUnk-&gt;QueryInterface(IID_IEnumVARIANT,(void**)&pEnum);
    if (FAILED(hr)) {
        fprintf(stderr, "Got an error getting the enumeration variant: %x", hr);
        return;
    }
    // Once we've got the interface we want, we can release the original one
    pUnk-&gt;Release();

    // Finally we can go round and enumerate the groups
    BSTR bstr;
    VARIANT var;
    IADs *pADs;
    ULONG lFetch;
    IDispatch *pDisp;

    // Initialise the variant
    VariantInit(&var);
    // Go and get the first group, hr will be S_OK if we get one
    hr = pEnum-&gt;Next(1, &var, &lFetch);
    while(hr == S_OK)
    {
        if (lFetch == 1)
        {
            // Now we need to get the IADs interface, so we use QueryInterface again
            pDisp = V_DISPATCH(&var);
            pDisp-&gt;QueryInterface(IID_IADs, (void**)&pADs);
            // Get the name of the group so that we can display it
            pADs-&gt;get_Name(&bstr);
            fprintf(stdout, "Group: %Sn",bstr);
            // Release the string because we don't need it any more
            SysFreeString(bstr);
            pADs-&gt;Release();
        }
        VariantClear(&var);
        // Now go and get the next one in the group
        hr = pEnum-&gt;Next(1, &var, &lFetch);
    };
    // We don't need the enumerator any more
    pEnum-&gt;Release();

    // We're done, so uninitialise the COM interface
    CoUninitialize();
}</pre>

The function starts off ordinarily enough by concatenating the string and then widening it then calling the `ADsGetObject`, from there, we can easily get the groups. Unfortunately, as was the case in JScript, once we&#8217;ve got the groups, we need to get an enumerator. We can do this by calling the romantically named  
[`get__NewEnum(&pUnk)`][4]  
(in fact this is method being called implicitly in both JScript and VBScript). Unfortunately, `get__NewEnum` only takes the address of an IUnknown pointer (the most general of interface types), so we then have to use [`QueryInterface`][5]  
to get the interface we want.

`QueryInterface` is the basic workhorse of COM and is equivalent to dynamic casting in other languages (and in fact is how languages that support automation actually do the dynamic casting). So, using `QueryInterface` we can as our IUnknown pointer for an `IEnumVARIANT` so that we can iterate through the groups.

Our enumerator has a `Next` method which allows us to get one or more members at a time, but since we don&#8217;t know how many there are, it&#8217;s easier to just get one at a time. The `Next` method also takes the address of a VARIANT. So, once we&#8217;ve got that, we need to call `V_DISPATCH` to get the `IDispatch` interface. Now we can use `QueryInterface` again to get the `IADs` interface. This is the most general of the ADSI interfaces and it has the basic `get_Name` that we need in order to be able to print out the name of the group.

Now that we have written the function, it&#8217;s easy to call this from a `main()` so that we can test it out. The complete program is downloadable [here][6]. For more information, see the  
[resources][7] section at the end of this article.

If you now run the program you should get something like the following:

<pre class="brush:text">c:cvsutility projectsjava_adsisrccpp&gt;getusergroups.exe DOLPHIN/denish
adsPath is 'WinNT://DOLPHIN/denish'
Starting up, calling CoInitialise
Now calling ADsGetObject
Group: Administrators
Group: Users</pre>

One disappointing observation is that there&#8217;s a noticable pause between printing out &#8220;Now calling ADsGetObject&#8221; and printing out the group names. Further investigation shows it is the actual call to `ADsGetObject` which takes the time. Taking 2 seconds each time we want to verify which groups a user is in probably isn&#8217;t going to be acceptable, on the other hand, caching the groups isn&#8217;t a good idea since we&#8217;d need to know when they were changed. Still, resolving this issue is outside the scope of this article. In [part 2][1] we&#8217;ll modify the function so as to be called from Java and wrap the whole thing in a JNI DLL.

## <a title="resources" name="resources"></a>Resources

  * [Download the VBScript][8] version in a ZIP file so that it doesn&#8217;t execute or worry your virus protection. Don&#8217;t forget to edit the script to change the username and domain or machine name.
  * [Download the ASP][9] version. Again in a zip, also you&#8217;ll need to have IIS configured if you want to use this version (and you&#8217;ll have to edit the file to change the username etc.).
  * [Download the C++][10] test program source.
  * [Instructions on using the (free) Borland C++ compiler][11] to compile the C++ test program

 [1]: 2005/05/adsi-from-java
 [2]: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/adsi/adsi/binding_with_getobject_and_adsgetobject.asp
 [3]: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/adsi/adsi/adsgetobject.asp
 [4]: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/adsi/adsi/iadscollection_get__newenum.asp
 [5]: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/htm/cmi_q2z_7fvp.asp
 [6]: getUserGroups.cpp
 [7]: #resources
 [8]: /articles/listGroups.vbs.zip
 [9]: /articles/listGroups.asp.zip
 [10]: /articles/getUserGroups.cpp
 [11]: 2005/03/free-borland-cpp/
