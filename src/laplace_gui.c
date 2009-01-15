/* 
 * Noise generator GIMP plugin
 * 
 * laplace_gui.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

void
laplace_gui (void)
{
  GtkRequisition req;

  /* stdev label */
  laplace_stdev_label = gtk_label_new (NULL);
  gtk_widget_set_tooltip_text(laplace_stdev_label, TT_LAPLACE_STDEV);
  gtk_widget_show (laplace_stdev_label);
  laplace_stdev_align = gtk_alignment_new (0, 0, 0, 0);
  gtk_container_add (GTK_CONTAINER (laplace_stdev_align),
		     laplace_stdev_label);
  gtk_widget_show (laplace_stdev_align);

  /* beta */
  laplace_beta_hbox = gtk_hbox_new (FALSE, 10);
  /* TRANSLATORS: this is the scale parameter of the Laplace distribution */
  laplace_beta_label = gtk_label_new (_("Scale"));
  laplace_beta_adj =
    gtk_adjustment_new (settings.laplace_scale, 0, 35, 0.1, 1, 0);
  g_signal_connect_swapped (laplace_beta_adj, "value-changed",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (laplace_beta_adj, "value-changed",
		    G_CALLBACK (set_laplace_scale), NULL);
  set_laplace_scale (GTK_ADJUSTMENT (laplace_beta_adj), NULL);
  laplace_beta_scale = gtk_hscale_new (GTK_ADJUSTMENT (laplace_beta_adj));
  gtk_scale_set_draw_value (GTK_SCALE (laplace_beta_scale), FALSE);
  gtk_widget_size_request (laplace_beta_scale, &req);
  gtk_widget_set_size_request (laplace_beta_scale, req.width * 3, req.height);
  laplace_beta_spin =
    gtk_spin_button_new (GTK_ADJUSTMENT (laplace_beta_adj), 0.1, 1);
  gtk_widget_set_tooltip_text (laplace_beta_label, TT_LAPLACE_SCALE);
  gtk_widget_set_tooltip_text (laplace_beta_spin, TT_LAPLACE_SCALE);
  gtk_widget_set_tooltip_text (laplace_beta_scale, TT_LAPLACE_SCALE);
  gtk_box_pack_start (GTK_BOX (laplace_beta_hbox), laplace_beta_label, FALSE,
		      FALSE, 0);
  gtk_box_pack_start (GTK_BOX (laplace_beta_hbox), laplace_beta_scale, TRUE,
		      TRUE, 0);
  gtk_box_pack_start (GTK_BOX (laplace_beta_hbox), laplace_beta_spin, FALSE,
		      FALSE, 0);
  gtk_widget_show (laplace_beta_hbox);
  gtk_widget_show (laplace_beta_label);
  gtk_widget_show (laplace_beta_scale);
  gtk_widget_show (laplace_beta_spin);

  notebook_box[3] = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_box[3]), 10);
  gtk_box_pack_start (GTK_BOX (notebook_box[3]), laplace_beta_hbox, FALSE,
		      FALSE, 0);
  gtk_box_pack_start (GTK_BOX (notebook_box[3]), laplace_stdev_align, FALSE,
		      FALSE, 0);
  gtk_widget_show (notebook_box[3]);
}
