/* 
 * Noise generator GIMP plugin
 * 
 * gauss_gui.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

void
lorentz_gui (void)
{
  GtkRequisition req;
  /* stdev */
  lorentz_gamma_hbox = gtk_hbox_new (FALSE, 10);
  /* TRANSLATORS: parameter of the Cauchy-Lorentz distribution
     (Greek letter) */
  lorentz_gamma_label = gtk_label_new (_("Gamma"));
  lorentz_gamma_adj =
    gtk_adjustment_new (settings.lorentz_gamma, 0, 30, 0.1, 1, 0);
  g_signal_connect_swapped (lorentz_gamma_adj, "value-changed",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (lorentz_gamma_adj, "value-changed",
		    G_CALLBACK (set_lorentz_gamma), NULL);
  lorentz_gamma_scale = gtk_hscale_new (GTK_ADJUSTMENT (lorentz_gamma_adj));
  gtk_scale_set_draw_value (GTK_SCALE (lorentz_gamma_scale), FALSE);
  gtk_widget_size_request (lorentz_gamma_scale, &req);
  gtk_widget_set_size_request (lorentz_gamma_scale, req.width * 3,
			       req.height);
  lorentz_gamma_spin =
    gtk_spin_button_new (GTK_ADJUSTMENT (lorentz_gamma_adj), 0.1, 1);
  gtk_widget_set_tooltip_text (lorentz_gamma_label, TT_LORENTZ_GAMMA);
  gtk_widget_set_tooltip_text (lorentz_gamma_spin, TT_LORENTZ_GAMMA);
  gtk_widget_set_tooltip_text (lorentz_gamma_scale, TT_LORENTZ_GAMMA);
  gtk_box_pack_start (GTK_BOX (lorentz_gamma_hbox), lorentz_gamma_label,
		      FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (lorentz_gamma_hbox), lorentz_gamma_scale, TRUE,
		      TRUE, 0);
  gtk_box_pack_start (GTK_BOX (lorentz_gamma_hbox), lorentz_gamma_spin, FALSE,
		      FALSE, 0);
  gtk_widget_show (lorentz_gamma_hbox);
  gtk_widget_show (lorentz_gamma_label);
  gtk_widget_show (lorentz_gamma_scale);
  gtk_widget_show (lorentz_gamma_spin);

  notebook_box[4] = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_box[4]), 10);
  gtk_box_pack_start (GTK_BOX (notebook_box[4]), lorentz_gamma_hbox, FALSE,
		      FALSE, 0);
  gtk_widget_show (notebook_box[4]);
}
