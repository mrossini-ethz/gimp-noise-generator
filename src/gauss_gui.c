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
gauss_gui (void)
{
  GtkRequisition req;
  /* stdev */
  gauss_stdev_hbox = gtk_hbox_new (FALSE, 10);
  /* TRANSLATORS: standard deviation (mathematical term) */
  gauss_stdev_label = gtk_label_new (_("Stdev"));
  gauss_stdev_adj =
    gtk_adjustment_new (settings.gauss_stdev, 0, 50, 0.1, 1, 0);
  g_signal_connect_swapped (gauss_stdev_adj, "value-changed",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (gauss_stdev_adj, "value-changed",
		    G_CALLBACK (set_gauss_stdev), NULL);
  gauss_stdev_scale = gtk_hscale_new (GTK_ADJUSTMENT (gauss_stdev_adj));
  gtk_scale_set_draw_value (GTK_SCALE (gauss_stdev_scale), FALSE);
  gtk_widget_size_request (gauss_stdev_scale, &req);
  gtk_widget_set_size_request (gauss_stdev_scale, req.width * 3, req.height);
  gauss_stdev_spin =
    gtk_spin_button_new (GTK_ADJUSTMENT (gauss_stdev_adj), 0.1, 1);
  gtk_widget_set_tooltip_text (gauss_stdev_label, TT_GAUSS_STDEV);
  gtk_widget_set_tooltip_text (gauss_stdev_spin, TT_GAUSS_STDEV);
  gtk_widget_set_tooltip_text (gauss_stdev_scale, TT_GAUSS_STDEV);
  gtk_box_pack_start (GTK_BOX (gauss_stdev_hbox), gauss_stdev_label, FALSE,
		      FALSE, 0);
  gtk_box_pack_start (GTK_BOX (gauss_stdev_hbox), gauss_stdev_scale, TRUE,
		      TRUE, 0);
  gtk_box_pack_start (GTK_BOX (gauss_stdev_hbox), gauss_stdev_spin, FALSE,
		      FALSE, 0);
  gtk_widget_show (gauss_stdev_hbox);
  gtk_widget_show (gauss_stdev_label);
  gtk_widget_show (gauss_stdev_scale);
  gtk_widget_show (gauss_stdev_spin);

  notebook_box[1] = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_box[1]), 10);
  gtk_box_pack_start (GTK_BOX (notebook_box[1]), gauss_stdev_hbox, FALSE,
		      FALSE, 0);
  gtk_widget_show (notebook_box[1]);
}
