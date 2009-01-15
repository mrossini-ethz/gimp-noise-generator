/* 
 * Noise generator GIMP plugin
 * 
 * poisson_gui.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

void
poisson_gui (void)
{
  GtkRequisition req;

  /* prepare the photon hbox */
  poisson_photon_hbox = gtk_hbox_new (FALSE, 10);
  poisson_photon_label = gtk_label_new (_("Photons"));
  gtk_misc_set_alignment (GTK_MISC (poisson_photon_label), 0.0, 0.0);
  poisson_photon_adj =
    gtk_adjustment_new (settings.poisson_photons, 0, 13, 1, 1, 0);
  poisson_photon_spin =
    gtk_spin_button_new (GTK_ADJUSTMENT (poisson_photon_adj), 1, 0);
  poisson_photon_scale = gtk_hscale_new (GTK_ADJUSTMENT (poisson_photon_adj));
  gtk_scale_set_draw_value (GTK_SCALE (poisson_photon_scale), FALSE);
  gtk_widget_size_request (poisson_photon_scale, &req);
  gtk_widget_set_size_request (poisson_photon_scale, req.width * 3,
			       req.height);
  gtk_box_pack_start (GTK_BOX (poisson_photon_hbox), poisson_photon_label,
		      FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (poisson_photon_hbox), poisson_photon_scale,
		      TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (poisson_photon_hbox), poisson_photon_spin,
		      FALSE, FALSE, 0);
  g_signal_connect_swapped (poisson_photon_adj, "value_changed",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (poisson_photon_adj, "value_changed",
		    G_CALLBACK (set_poisson_photons), NULL);
  gtk_widget_show (poisson_photon_scale);
  gtk_widget_show (poisson_photon_spin);
  gtk_widget_show (poisson_photon_label);
  gtk_widget_set_tooltip_text (poisson_photon_label, TT_POISSON_PHOTON);
  gtk_widget_set_tooltip_text (poisson_photon_scale, TT_POISSON_PHOTON);
  gtk_widget_set_tooltip_text (poisson_photon_spin, TT_POISSON_PHOTON);
  gtk_widget_show (poisson_photon_hbox);

  /* approximation toggle */
  poisson_approx_toggle = gtk_check_button_new_with_label (_("Approximate"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (poisson_approx_toggle),
				settings.poisson_approx);
  gtk_widget_set_tooltip_text (poisson_approx_toggle, TT_POISSON_APPROX);
  g_signal_connect_swapped (poisson_approx_toggle, "toggled",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (poisson_approx_toggle, "toggled",
		    G_CALLBACK (set_poisson_approx), NULL);
  gtk_widget_show (poisson_approx_toggle);

  /* prepare the reset buttons */
  /*poisson_button_hbox = gtk_hbox_new (FALSE, 10);
     poisson_reset_button = gtk_button_new_from_stock (GIMP_STOCK_RESET);
     g_signal_connect (poisson_reset_button, "clicked",
     G_CALLBACK (poisson_reset), NULL);
     gtk_widget_set_tooltip_text (poisson_reset_button, TT_RESET);
     gtk_widget_show (poisson_reset_button);
     gtk_widget_show (poisson_button_hbox); */

  /* prepare the poisson vbox */
  notebook_box[0] = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_box[0]), 10);
  gtk_box_pack_start (GTK_BOX (notebook_box[0]), poisson_photon_hbox, FALSE,
		      FALSE, 0);
  gtk_box_pack_start (GTK_BOX (notebook_box[0]), poisson_approx_toggle,
		      TRUE, TRUE, 0);
  gtk_widget_show (notebook_box[0]);
}
