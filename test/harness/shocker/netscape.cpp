/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * netscape.cpp: Mozilla plugin entry point functions
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007-2010 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 *
 */

#include <dlfcn.h>

#include "netscape.h"
#include "shocker.h"
#include "plugin.h"
#include "browser.h"
#include "shutdown-manager.h"

//
// These are the functions that mozilla looks up and is going to call to figure out
// what kind of plugin we are and to instantiate/destroy us
//

NPError
NP_Initialize (NPNetscapeFuncs* mozilla_funcs, NPPluginFuncs* plugin_funcs)
{
#ifdef SHOCKER_DEBUG
	printf ("NP_Initialize\n");
#endif

	Dl_info dl_info;
	// Prevent firefox from unloading us
	if (dladdr ((void *) &NP_Initialize, &dl_info) != 0) {
		void *handle = dlopen (dl_info.dli_fname, RTLD_LAZY | RTLD_NOLOAD);
		if (handle == NULL)
			printf ("[shocker] tried to open a handle to libshocker.so, but: '%s' (rare crashes might occur).\n", dlerror ());
	} else {
		printf ("[shocker] could not get path of libshocker.so: '%s' (rare crashes might occur).\n", dlerror ());
	}
	
	Browser_Initialize (mozilla_funcs);
	Plugin_Initialize (plugin_funcs);
	Shocker_Initialize ();

	return NPERR_NO_ERROR;
}

NPError
NP_Shutdown (void)
{
#ifdef SHOCKER_DEBUG
    printf ("NP_Shutdown\n");
#endif

    Shocker_Shutdown ();
    
    return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription (void)
{
#ifdef SHOCKER_DEBUG
	printf ("NP_GetMIMEDescription\n");
#endif

	return Plugin_GetMIMEDescription ();
}

NPError
NP_GetValue (void *future, NPPVariable variable, void *value)
{
#ifdef SHOCKER_DEBUG
	printf ("NP_GetValue\n");
#endif

	return Plugin_GetValue ((NPP) future, variable, value);
}

