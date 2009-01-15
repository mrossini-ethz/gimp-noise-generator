/* 
 * Noise generator GIMP plugin
 * 
 * plugin.h
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <stdlib.h>
#include <math.h>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <libintl.h>
#include <locale.h>

#include "messages.h"

#define GIMP_PLUGIN_ID "plug-in-noise-generator"
#define GETTEXT_ID "gimp20-noise-generator-plug-in"

#define LOCALEDIR "/usr/share/locale/"

#define _(string) gettext(string)
#define N_(string) string

#define MAX2(x,y) ((x) > (y) ? (x) : (y))
#define MIN2(x,y) ((x) < (y) ? (x) : (y))
#define CLIP(x,min,max) MAX2((min), MIN2((x), (max)))

void query (void);
void run (const gchar * name, gint nparams, const GimpParam * param,
	  gint * nreturn_vals, GimpParam ** return_vals);
void noise_gen (GimpDrawable * drawable, GimpPreview * preview);
gboolean user_interface (GimpDrawable * drawable);
void rgb2ycbcr (float *r, float *g, float *b);
void ycbcr2rgb (float *y, float *cb, float *cr);
void set_preview_progress (double frac);

extern GimpPlugInInfo PLUG_IN_INFO;

typedef struct
{
  guint poisson_photons;
  gboolean poisson_approx;

  float gauss_stdev;
  float uniform_width;
  float laplace_scale;
  float lorentz_gamma;

  gboolean luminance_only;
  gboolean correct_gamma;
  gboolean show_preview;
  gint lastpage;
  gint winxsize, winysize;
} generator_settings;

extern generator_settings settings;

gint channels;

#endif /* __PLUGIN_H__ */
