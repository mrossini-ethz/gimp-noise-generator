/* 
 * Noise generator GIMP plugin
 * 
 * plugin.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"

GimpPlugInInfo PLUG_IN_INFO = { NULL, NULL, query, run };

MAIN ()
     void query (void)
{
  static GimpParamDef args[] = {
    {GIMP_PDB_INT32, "run-mode", "Run mode"},
    {GIMP_PDB_IMAGE, "image", "Input image"},
    {GIMP_PDB_DRAWABLE, "drawable", "Input drawable"},
    {GIMP_PDB_INT8, "poisson photons", "Poisson: number of photons"},
    {GIMP_PDB_INT8, "poisson approx", "Poisson: use approximation"},
    {GIMP_PDB_FLOAT, "gauss stdev", "Gauss: standard deviation"},
    {GIMP_PDB_INT8, "uniform width", "Uniform: width"},
    {GIMP_PDB_INT8, "luminance", "Luminance only"},
    {GIMP_PDB_INT8, "gamma", "Correct gamma"}
  };

  gimp_install_procedure (GIMP_PLUGIN_ID,
			  N_("Generates various types of noise."),
			  "Generates various types of noise.",
			  "Marco Rossini",
			  "Copyright 2008 Marco Rossini",
			  "2008",
			  N_("_Noise generator..."),
			  "RGB*, GRAY*",
			  GIMP_PLUGIN, G_N_ELEMENTS (args), 0, args, NULL);

  gimp_plugin_domain_register (GETTEXT_ID, LOCALEDIR);

  gimp_plugin_menu_register (GIMP_PLUGIN_ID, "<Image>/Filters/Noise/");
}

generator_settings settings = {
  0,				/* poisson_photons */
  FALSE,			/* poisson_approx */
  0.0,				/* gauss_stdev */
  0.0,				/* uniform_width */
  0.0,				/* laplace_scale */
  0.0,				/* lorentz_gamma */
  TRUE,				/* luminance_only */
  FALSE,			/* correct_gamma */
  TRUE,				/* show_preview */
  0,				/* lastpage */
  -1, -1			/* winxsize, winysize */
};

void
run (const gchar * name, gint nparams, const GimpParam * param,
     gint * nreturn_vals, GimpParam ** return_vals)
{
  static GimpParam values[1];
  GimpRunMode run_mode;
  GimpDrawable *drawable;

  bindtextdomain (GETTEXT_ID, LOCALEDIR);
  textdomain (GETTEXT_ID);
  bind_textdomain_codeset (GETTEXT_ID, "UTF-8");

  /* Setting mandatory output values */
  *nreturn_vals = 1;
  *return_vals = values;
  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = GIMP_PDB_SUCCESS;

  /* restore settings saved in GIMP core */
  gimp_get_data (GIMP_PLUGIN_ID, &settings);

  drawable = gimp_drawable_get (param[2].data.d_drawable);
  channels = gimp_drawable_bpp (drawable->drawable_id);

  /* run GUI if in interactiv mode */
  run_mode = param[0].data.d_int32;
  if (run_mode == GIMP_RUN_INTERACTIVE)
    {
      if (!user_interface (drawable))
	{
	  gimp_drawable_detach (drawable);
	  values[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;
	  return;
	}
    }
  else
    {
      if (nparams == 9)
	{
	  int invalid = 0;
	  settings.poisson_photons = param[3].data.d_int8;
	  settings.poisson_approx = !!(param[4].data.d_int8);
	  settings.gauss_stdev = param[5].data.d_float;
	  settings.uniform_width = param[6].data.d_int8;
	  settings.luminance_only = !!(param[7].data.d_int8);
	  settings.correct_gamma = !!(param[8].data.d_int8);

	  /* check validity of parameters */
	  invalid = invalid || settings.poisson_photons < 0;
	  invalid = invalid || settings.poisson_photons > 13;
	  invalid = invalid || settings.gauss_stdev < 0;
	  invalid = invalid || settings.gauss_stdev > 50;
	  invalid = invalid || settings.uniform_width < 0;
	  invalid = invalid || settings.uniform_width > 100;
	  if (invalid)
	    {
	      gimp_drawable_detach (drawable);
	      values[0].data.d_status = GIMP_PDB_CALLING_ERROR;
	      return;
	    }
	}
    }

  noise_gen (drawable, NULL);

  gimp_displays_flush ();
  gimp_drawable_detach (drawable);

  /* save settings in the GIMP core */
  gimp_set_data (GIMP_PLUGIN_ID, &settings, sizeof (generator_settings));
}
