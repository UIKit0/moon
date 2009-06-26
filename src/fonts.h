/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * fonts.h: 
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2009 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#ifndef __FONTS_H__
#define __FONTS_H__

#include <glib.h>
#include <cairo.h>

#include "fontmanager.h"
#include "enums.h"

#define GLYPH_CACHE_SIZE 256

class TextFontDescription;

class TextFont {
	const TextFontDescription *desc;
	StyleSimulations simulate;
	FontFaceExtents extents;
	FontFace **faces;
	int n_faces;
	double size;
	
	GlyphInfo glyphs[GLYPH_CACHE_SIZE];
	int n_glyphs;
	
	TextFont (FontFace **faces, int n_faces, double size);
	
	GlyphInfo *GetGlyphInfo (FontFace *face, gunichar unichar, guint32 index);
	void ClearGlyphCache ();
	
 public:
	~TextFont ();
	
	static TextFont *Load (const char *resource, int index, double size, StyleSimulations simulate);
	static TextFont *Load (const TextFontDescription *desc);
	
	bool SetStyleSimulations (StyleSimulations simulate);
	StyleSimulations GetStyleSimulations ();
	
	bool SetSize (double size);
	double GetSize ();
	
	GlyphInfo *GetGlyphInfo (gunichar unichar);
	GlyphInfo *GetGlyphInfoByIndex (guint32 index);
	
	double Kerning (guint32 left, guint32 right);
	double Descender ();
        double Ascender ();
	double Height ();
	
	double UnderlinePosition ();
	double UnderlineThickness ();
	
	void Path (cairo_t *cr, GlyphInfo *glyph, double x, double y);
	void Path (cairo_t *cr, gunichar unichar, double x, double y);
	
	void AppendPath (moon_path *path, GlyphInfo *glyph, double x, double y);
	void AppendPath (moon_path *path, gunichar unichar, double x, double y);
};


class TextFontDescription {
	TextFont *font;
	bool changed;
	
	char *source;
	char *family;
	FontStretches stretch;
	FontWeights weight;
	FontStyles style;
	double size;
	
 public:
	TextFontDescription ();
	~TextFontDescription ();
	
	TextFont *GetFont ();
	void Reload ();
	
	bool SetSource (const char *source);
	const char *GetSource () const;
	
	bool SetFamily (const char *family);
	const char *GetFamily () const;
	char **GetFamilies () const;
	
	bool SetStretch (FontStretches stretch);
	FontStretches GetStretch () const;
	
	bool SetWeight (FontWeights weight);
	FontWeights GetWeight () const;
	
	bool SetStyle (FontStyles style);
	FontStyles GetStyle () const;
	
	bool SetSize (double size);
	double GetSize () const;
};

#endif /* __FONTS_H__ */
