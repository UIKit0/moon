/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * shutdown-manager.cpp: When shutdown is signaled, the shutdown manager
 * 			 will wait until everything (image capture) is done
 *			 before it will actually shutdown the app.
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007-2010 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 *
 */

#include <glib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shutdown-manager.h"
#include "input.h"
#include "plugin.h"

static gint wait_count = 0;

static void execute_shutdown ();
static gboolean attempt_clean_shutdown (gpointer data);

void
shutdown_manager_wait_increment ()
{
	g_atomic_int_inc (&wait_count);
}

void
shutdown_manager_wait_decrement ()
{
	g_atomic_int_dec_and_test (&wait_count);
}

static gboolean
force_shutdown (gpointer data)
{
	if (getenv ("MOONLIGHT_SHOCKER_DONT_EXIT") == NULL) {
		printf ("[shocker] Could not shutdown nicely, exiting the process.\n");
		exit (0);
	} else {
		printf ("[shocker] Could not shutdown nicely, but won't exit process since MOONLIGHT_SHOCKER_DONT_EXIT is set.\n");
	}
	return false;
}

static gboolean
send_alt_f4 (gpointer dummy)
{
	printf ("[%i shocker] sending Alt+F4 to firefox...\n", getpid ());
	InputProvider *input = InputProvider::GetInstance ();
	// send alt-f4
	input->SendKeyInput (VK_MENU, true, false, false);
	input->SendKeyInput (VK_F4, true, false, false);
	input->SendKeyInput (VK_F4, false, false, false);
	input->SendKeyInput (VK_MENU, false, false, false);
	return false;
}

static gboolean
send_ctrl_q (gpointer dummy)
{
	printf ("[%i shocker] sending Ctrl-Q to firefox...\n", getpid ());
	// This doesn't work with oob - there is no menu so nothing handles ctrl-q
	InputProvider *input = InputProvider::GetInstance ();
	// send ctrl-q
	input->SendKeyInput (VK_CONTROL, true, false, false);
	input->SendKeyInput (VK_Q, true, false, false);
	input->SendKeyInput (VK_Q, false, false, false);
	input->SendKeyInput (VK_CONTROL, false, false, false);
	return false;
}

static void
execute_shutdown ()
{
	char *dont_die = getenv ("MOONLIGHT_SHOCKER_DONT_DIE");
	if (dont_die != NULL && dont_die [0] != 0)
		return;

	g_type_init ();

	if (PluginObject::browser_app_context != 0) {
		printf ("[shocker] shutting down firefox...\n");

		Display *display = XOpenDisplay (NULL);
		Atom WM_PROTOCOLS = XInternAtom (display, "WM_PROTOCOLS", False);
		Atom WM_DELETE_WINDOW = XInternAtom (display, "WM_DELETE_WINDOW", False);
		XClientMessageEvent ev;
		
		ev.type = ClientMessage;
		ev.window = PluginObject::browser_app_context;
		ev.message_type = WM_PROTOCOLS;
		ev.format = 32;
		ev.data.l [0] = WM_DELETE_WINDOW;
		ev.data.l [1] = 0;
		
		XSendEvent (display, ev.window, False, 0, (XEvent*) &ev);
		XCloseDisplay (display);
		
		PluginObject::browser_app_context = 0;

		// have a backup, since the above doesn't work with oob
		g_timeout_add (1000, send_alt_f4, NULL);
	} else {
		send_ctrl_q (NULL); // this doesn't work for oob apps
		send_alt_f4 (NULL); // this doesn't work if there is no window manager (inside xvfb while running tests for instance)
	}
	
	// Have a backup in case the above fails.
	g_timeout_add (25000, force_shutdown, NULL);
}

static gboolean
attempt_clean_shutdown (gpointer data)
{
	bool ready_for_shutdown = false;

	ready_for_shutdown = g_atomic_int_get (&wait_count) <= 0;

	if (ready_for_shutdown) {
		execute_shutdown ();
		return FALSE;
	}

	return TRUE;
}

void
shutdown_manager_queue_shutdown ()
{
	if (g_atomic_int_get (&wait_count) == 0)
		return execute_shutdown ();

	printf ("[shocker] Unable to execute shutdown immediately (pending screenshots), attempting a clean shutdown.\n");
	
	if (!g_timeout_add (100, attempt_clean_shutdown, NULL)) {
		printf ("[shocker] Unable to create timeout for queued shutdown, executing immediate shutdown.\n");
		execute_shutdown ();
	}
}

void
SignalShutdown (const char *window_name)
{
	shutdown_manager_queue_shutdown ();
}

void
TestHost_SignalShutdown (const char *window_name)
{
	SignalShutdown (window_name);
}
