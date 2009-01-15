/* 
 * Noise generator GIMP plugin
 * 
 * noise.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"

#define PI 3.141592653589793238462643

float
uniform (float mu, float width)
{
  double u;
  u = rand () / (double) RAND_MAX;
  return mu + width * (u - 0.5);
}

float
gauss (float mu, float sigma2)
{
  double u, v;
  while ((u = rand () / (double) RAND_MAX) == 0.0);
  v = rand () / (double) RAND_MAX;
  /* FIXME: rounding bias here! */
  return mu + sigma2 * sqrt (-2 * log (u)) * cos (2 * PI * v);
}

float
laplace (float mu, float b)
{
  double u = 0.0;
  while (u <= 0.0 || u >= 1.0)
    u = rand () / (double) RAND_MAX;
  u -= 0.5;
  if (u > 0)
    return mu + b * log (1 - 2 * u);
  else
    return mu - b * log (1 + 2 * u);
}

float
poisson (float lambda, guint n)
{
  double L, p;
  guint k;

  if (n == 0)
    return 0;

  /* FIXME: this might clip */
  if (settings.poisson_approx && lambda > 40)
    return gauss (lambda * n, sqrt (lambda * n));

  L = exp (-lambda);
  k = 0;
  p = 1;
  do
    {
      k++;
      p = p * rand () / (double) RAND_MAX;
    }
  while (p >= L && k <= 255);
  return k - 1 + poisson (lambda, n - 1);
}

float
lorentz (float x0, float gamma)
{
  double u = 0.0;
  while (u <= 0.0 || u >= 1.0)
    u = rand () / (double) RAND_MAX;
  return x0 + gamma * tan (PI * (u - 0.5));
}

void
noise_gen (GimpDrawable * drawable, GimpPreview * preview)
{
  GimpPixelRgn rgn_in, rgn_out;
  gint i, x1, y1, x2, y2, width, height, x, c;
  guchar *line;
  float val[4];

  if (preview)
    {
      gimp_preview_get_position (preview, &x1, &y1);
      gimp_preview_get_size (preview, &width, &height);
      x2 = x1 + width;
      y2 = y1 + height;
    }
  else
    {
      gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
      width = x2 - x1;
      height = y2 - y1;
    }

  gimp_pixel_rgn_init (&rgn_in, drawable, x1, y1, width, height, FALSE,
		       FALSE);
  gimp_pixel_rgn_init (&rgn_out, drawable, x1, y1, width, height,
		       preview == NULL, TRUE);

  /* cache some tiles to make reading/writing faster */
  gimp_tile_cache_ntiles (2 * drawable->width / gimp_tile_width () + 1);

  /* FIXME: replace by GIMP functions */
  line = (guchar *) malloc (channels * width * sizeof (guchar));

  /* read, generate and write */
  if (!preview)
    gimp_progress_init (_("Generating noise..."));
  for (i = 0; i < y2 - y1; i++)
    {
      if (!preview && i % 10 == 0)
	{
	  gimp_progress_update (i / (double) height);
	}
      gimp_pixel_rgn_get_row (&rgn_in, line, x1, i + y1, width);

      for (x = 0; x < width; x++)
	{
	  for (c = 0; c < channels; c++)
	    {
	      val[c] = (float) line[x * channels + c];
	      /* FIXME: check whether luminance only */
	    }
	  /* must be before gamma conversion */
	  if (channels > 2 && settings.luminance_only)
	    rgb2ycbcr (&(val[0]), &(val[1]), &(val[2]));
	  if (settings.correct_gamma)
	    {
	      for (c = 0; c < channels; c++)
		{
		  /* real sRGB --> linear RGB */
		  if (val[c] <= 0.04045 * 255)
		    val[c] /= 12.92;
		  else
		    val[c] =
		      pow ((val[c] / 255.0 + 0.055) / 1.055, 2.4) * 255.0;
		}
	    }
	  /* FIXME: dirty hack in for condition */
	  for (c = 0;
	       c <= (channels - 1) * (channels < 3
				      || !settings.luminance_only); c++)
	    {
	      if (settings.poisson_photons > 0)
		{
		  val[c] = poisson (val[c], settings.poisson_photons);
		  val[c] /= (double) settings.poisson_photons;
		}
	      if (settings.gauss_stdev > 0)
		val[c] = gauss (val[c], settings.gauss_stdev);
	      if (settings.uniform_width > 0)
		val[c] = uniform (val[c], settings.uniform_width);
	      if (settings.laplace_scale > 0)
		val[c] = laplace (val[c], settings.laplace_scale);
	      if (settings.lorentz_gamma > 0)
		val[c] = lorentz (val[c], settings.lorentz_gamma);
	    }
	  if (settings.correct_gamma)
	    {
	      val[c] = CLIP (val[c], 0, 255);
	      for (c = 0; c < channels; c++)
		{
		  /* linear RGB --> real sRGB */
		  if (val[c] <= 0.0031308 * 255)
		    val[c] *= 12.92;
		  else
		    val[c] =
		      (pow (val[c] / 255.0, 1 / 2.4) * 1.055 - 0.055) * 255.0;
		}
	    }
	  if (channels > 2 && settings.luminance_only)
	    ycbcr2rgb (&(val[0]), &(val[1]), &(val[2]));
	  for (c = 0; c < channels; c++)
	    {
	      line[x * channels + c] = (guchar) (CLIP (val[c], 0, 255) + 0.5);
	    }
	}
      gimp_pixel_rgn_set_row (&rgn_out, line, x1, i + y1, width);
    }

  /* FIXME: replace by gimp functions */
  free (line);

  if (preview)
    {
      gimp_drawable_preview_draw_region (GIMP_DRAWABLE_PREVIEW (preview),
					 &rgn_out);
      return;
    }
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, width, height);
}
