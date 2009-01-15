/* 
 * Noise generator GIMP plugin
 * 
 * interface.h
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"

/* event functions */
void set_ycbcr_mode (GtkWidget * w, gpointer data);
void set_gamma (GtkWidget * w, gpointer data);
void reset (GtkWidget * w, gpointer data);
void set_poisson_photons (GtkAdjustment * w, gpointer data);
void set_poisson_approx (GtkWidget * w, gpointer data);
void set_gauss_stdev (GtkAdjustment * w, gpointer data);
void set_uniform_width (GtkAdjustment * w, gpointer data);
void set_laplace_scale (GtkAdjustment * w, gpointer data);
void set_lorentz_gamma (GtkAdjustment * w, gpointer data);
void change_page (GtkWidget * w, GtkNotebookPage * p, guint no,
		  gpointer data);

/* gui functions */
void poisson_gui (void);
void gauss_gui (void);
void uniform_gui (void);
void laplace_gui (void);
void lorentz_gui (void);

/* declare all widgets global to make things a lot easier */

/* poisson photon */
GtkWidget *poisson_photon_label, *poisson_photon_spin;
GtkWidget *poisson_photon_hbox, *poisson_photon_scale;
GtkObject *poisson_photon_adj;

/* poisson buttons */
GtkWidget *poisson_toggle_vbox;
GtkWidget *poisson_approx_toggle;

/* gauss stdev */
GtkObject *gauss_stdev_adj;
GtkWidget *gauss_stdev_hbox, *gauss_stdev_scale,
  *gauss_stdev_spin, *gauss_stdev_label;

/* uniform width */
GtkObject *uniform_width_adj;
GtkWidget *uniform_width_hbox, *uniform_width_scale,
  *uniform_width_spin, *uniform_width_label;
GtkWidget *uniform_stdev_align, *uniform_stdev_label;

/* laplace beta */
GtkObject *laplace_beta_adj;
GtkWidget *laplace_beta_hbox, *laplace_beta_scale,
  *laplace_beta_spin, *laplace_beta_label;
GtkWidget *laplace_stdev_align, *laplace_stdev_label;

/* gauss stdev */
GtkObject *lorentz_gamma_adj;
GtkWidget *lorentz_gamma_hbox, *lorentz_gamma_scale,
  *lorentz_gamma_spin, *lorentz_gamma_label;

/* notebook */
GtkWidget *notebook;
GtkWidget *notebook_label[5];
GtkWidget *notebook_box[5];

/* toggles */
GtkWidget *toggle_vbox;
GtkWidget *lum_toggle;
GtkWidget *gamma_toggle;

/* buttons */
GtkWidget *button_hbox;
GtkWidget *reset_button;

/* dialog */
GtkWidget *dialog, *dialog_vbox, *dialog_hbox;
GtkWidget *preview, *preview_toggle;
