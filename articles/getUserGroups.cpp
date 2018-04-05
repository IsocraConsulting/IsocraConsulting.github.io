#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <lm.h>
#include <activeds.h>
#include <initguid.h>

/***************************************************************************
* Print out the groups for the specified user
***************************************************************************/
void getGroupsFor(const char *user)
{
	// First we have to make up the string that we're going to pass to ADsGetObject
	char adsPath[80];
	strcat(adsPath, "WinNT://");
	strcat(adsPath, user);
	fprintf(stdout, "adsPath is '%s'\n", adsPath);

	// Call CoInitialize to initialise COM
	fprintf(stdout, "Starting up, calling CoInitialise\n");
	CoInitialize(NULL);

	// Convert the adsPath to wide chars so that we can pass it to ADsGetObject
    int nLen = strlen(adsPath) + 1;
    WCHAR* widePath = new WCHAR [nLen];
    mbstowcs(widePath, adsPath, nLen);

	// Now set up the required variables and call ADSGetObject
	fprintf(stdout, "Now calling ADsGetObject\n");
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
    hr = pUser->Groups(&pGroups);
	if(FAILED(hr)) {
		fprintf(stderr, "Got an error getting the groups: %x", hr);
		return;
	}
	// Don't need the user any more
    pUser->Release();

	// In order to iterate through the groups we need to get an enumerator
	// We then don't need the groups object itself any more so we can release it
    IUnknown *pUnk;
    hr = pGroups->get__NewEnum(&pUnk);
    if (FAILED(hr)) {
		fprintf(stderr, "Got an error getting the enumeration: %x", hr);
		return;
	}
    pGroups->Release();

	// From IUnknown interface, we can query to get the EnumVARIANT interface
    IEnumVARIANT *pEnum;
    hr = pUnk->QueryInterface(IID_IEnumVARIANT,(void**)&pEnum);
    if (FAILED(hr)) {
		fprintf(stderr, "Got an error getting the enumeration variant: %x", hr);
		return;
	}
	// Once we've got the interface we want, we can release the original one
    pUnk->Release();

    // Finally we can go round and enumerate the groups
    BSTR bstr;
    VARIANT var;
    IADs *pADs;
    ULONG lFetch;
    IDispatch *pDisp;

	// Initialise the variant
    VariantInit(&var);
    // Go and get the first group, hr will be S_OK if we get one
    hr = pEnum->Next(1, &var, &lFetch);
    while(hr == S_OK)
    {
        if (lFetch == 1)
        {
			// Now we need to get the IADs interface, so we use QueryInterface again
			pDisp = V_DISPATCH(&var);
			pDisp->QueryInterface(IID_IADs, (void**)&pADs);
			// Get the name of the group so that we can display it
			pADs->get_Name(&bstr);
			fprintf(stdout, "Group: %S\n",bstr);
			// Release the string because we don't need it any more
			SysFreeString(bstr);
			pADs->Release();
        }
        VariantClear(&var);
        // Now go and get the next one in the group
        hr = pEnum->Next(1, &var, &lFetch);
    };
    // We don't need the enumerator any more
    pEnum->Release();

	// We're done, so uninitialise the COM interface
	CoUninitialize();
}

/***************************************************************************
* Main function called to test our ADSI link
***************************************************************************/
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage getUserGroups <domain>/<username>\nWhere <domain> can be either a domain or a machine name");
	} else {
		getGroupsFor(argv[1]);
	}
}

/**
 * $Log: getUserGroups.cpp,v $
 * Revision 1.2  2007/05/16 16:13:09  denish
 * Add drag and drop tables
 *
 * Revision 1.1  2004/05/13 18:06:55  denish
 * Adding updated ADSI stuff
 *
 * Revision 1.1  2004/05/13 17:09:23  denish
 * Adding this project
 *
 * Revision 1.1  2004/05/13 17:05:08  denish
 * Adding this project
 *
 */

