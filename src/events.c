/* 
 * Noise generator GIMP plugin
 * 
 * events.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

void
reset (GtkWidget * w, gpointer data)
{
  settings.poisson_photons = 0;
  gtk_adjustment_set_value (GTK_ADJUSTMENT (poisson_photon_adj), 0);
  settings.poisson_approx = FALSE;
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (poisson_approx_toggle),
				FALSE);
  settings.gauss_stdev = 0.0;
  gtk_adjustment_set_value (GTK_ADJUSTMENT (gauss_stdev_adj), 0);
  settings.uniform_width = 0.0;
  gtk_adjustment_set_value (GTK_ADJUSTMENT (uniform_width_adj), 0);
  settings.laplace_scale = 0.0;
  gtk_adjustment_set_value (GTK_ADJUSTMENT (laplace_beta_adj), 0);
  settings.lorentz_gamma = 0.0;
  gtk_adjustment_set_value (GTK_ADJUSTMENT (lorentz_gamma_adj), 0);

  settings.luminance_only = TRUE;
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (lum_toggle), TRUE);
  settings.correct_gamma = FALSE;
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gamma_toggle), FALSE);
}

void
set_ycbcr_mode (GtkWidget * w, gpointer data)
{
  settings.luminance_only =
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (w));
}

void
set_gamma (GtkWidget * w, gpointer data)
{
  settings.correct_gamma =
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (w));
}

void
set_poisson_approx (GtkWidget * w, gpointer data)
{
  settings.poisson_approx =
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (w));
}

void
set_poisson_photons (GtkAdjustment * w, gpointer data)
{
  double val;
  val = floor (gtk_adjustment_get_value (GTK_ADJUSTMENT (w)) + 0.5);
  settings.poisson_photons = val;
}

void
set_gauss_stdev (GtkAdjustment * w, gpointer data)
{
  double val;
  val = gtk_adjustment_get_value (GTK_ADJUSTMENT (w));
  settings.gauss_stdev = val;
}

void
set_uniform_width (GtkAdjustment * w, gpointer data)
{
  double val;
  val = floor (gtk_adjustment_get_value (GTK_ADJUSTMENT (w)) + 0.5);
  settings.uniform_width = val;

  gchar stdev[51];
  /* TRANSLATORS: %.2f must remain unchanged. Maximum length of entire
     string is 50 chars. */
  sprintf (stdev, _("Standard deviation: %.2f"), sqrt (val * val / 12.0));
  gtk_label_set_text (GTK_LABEL (uniform_stdev_label), stdev);
}

void
set_laplace_scale (GtkAdjustment * w, gpointer data)
{
  double val;
  val = gtk_adjustment_get_value (GTK_ADJUSTMENT (w));
  settings.laplace_scale = val;

  gchar stdev[51];
  /* TRANSLATORS: %.2f must remain unchanged. Maximum length of entire
     string is 50 chars. */
  sprintf (stdev, _("Standard deviation: %.2f"), 1.414213562 * val);
  gtk_label_set_text (GTK_LABEL (laplace_stdev_label), stdev);
}

void
set_lorentz_gamma (GtkAdjustment * w, gpointer data)
{
  double val;
  val = gtk_adjustment_get_value (GTK_ADJUSTMENT (w));
  settings.lorentz_gamma = val;
}

void
change_page (GtkWidget * w, GtkNotebookPage * p, guint no, gpointer data)
{
  settings.lastpage = no;
}
