/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * multiscaleimage.h:
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#ifndef __MULTISCALEIMAGE_H__
#define __MULTISCALEIMAGE_H__

#include <glib.h>
#include <cairo.h>

#include "multiscalesubimage.h"
#include "tilesource.h"
#include "eventargs.h"
#include "control.h"
#include "media.h"

void multi_scale_image_handle_parsed (void *userdata);

/* @SilverlightVersion="2" */
/* @Namespace=System.Windows.Controls,ManagedDependencyProperties=Manual */
class MultiScaleImage : public MediaBase {
	void DownloaderAbort ();
	void DownloadUri (const char* url);
	GHashTable *cache;
	bool cache_contains (int layer, int x, int y, int subimage_id, bool check_empty_tile);
	MultiScaleTileSource *source;
	char* context;
	char* filename;
	bool downloading;

	cairo_user_data_key_t width_key;
	cairo_user_data_key_t height_key;

	void DownloaderComplete ();
	static void downloader_complete (EventObject *sender, EventArgs *calldata, gpointer closure);
	void DownloaderFailed ();
	static void downloader_failed (EventObject *sender, EventArgs *calldata, gpointer closure);

	Downloader *downloader;
	void RenderCollection (cairo_t *cr, Region *region);

 protected:
	virtual ~MultiScaleImage ();

 public:
	static void EmitImageOpenSucceeded (EventObject *user_data);

	/* @PropertyType=double,DefaultValue=1.0,Version=2.0,GenerateGetter */
	const static int AspectRatioProperty;
	/* @PropertyType=MultiScaleTileSource,Version=2.0,GenerateAccessors */
	const static int SourceProperty;
	/* @PropertyType=MultiScaleSubImageCollection,AutoCreateValue,ReadOnly,Version=2.0,GenerateGetter */
	const static int SubImagesProperty;
	/* @PropertyType=bool,DefaultValue=true,Version=2.0,GenerateAccessors */
	const static int UseSpringsProperty;
	/* @PropertyType=Point,DefaultValue=Point(0\,0),Version=2.0,GenerateAccessors */
	const static int ViewportOriginProperty;
	/* @PropertyType=double,DefaultValue=1.0,Version=2.0,GenerateAccessors */
	const static int ViewportWidthProperty;

	/* @GenerateCBinding,GeneratePInvoke */
	MultiScaleImage ();

	//
	// Overrides
	//
	virtual void Render (cairo_t *cr, Region *region, bool path_only = false);
	virtual void OnPropertyChanged (PropertyChangedEventArgs *args);

	//
	// Methods
	//
	/* @GenerateCBinding,GeneratePInvoke */
	void ZoomAboutLogicalPoint (double zoomIncrementFactor, double zoomCenterLogicalX, double zoomCenterLogicalY);
	/* @GenerateCBinding,GeneratePInvoke */
	Point ElementToLogicalPoint (Point elementPoint);

	//
	// Property Accessors
	//
	double GetAspectRatio ();

	MultiScaleTileSource* GetSource ();
	void SetSource (MultiScaleTileSource* source);

	bool GetUseSprings ();
	void SetUseSprings (bool spring);

	Point* GetViewportOrigin ();
	void SetViewportOrigin (Point* p);

	double GetViewportWidth ();
	void SetViewportWidth (double width);

	MultiScaleSubImageCollection *GetSubImages ();

	//
	// Events
	//
	const static int ImageFailedEvent;
	const static int ImageOpenFailedEvent;
	const static int ImageOpenSucceededEvent;
	const static int MotionFinishedEvent;
	const static int ViewportChangedEvent;
};

#endif /* __MULTISCALIMAGE_H__ */
