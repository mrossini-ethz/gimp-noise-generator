/* 
 * Noise generator GIMP plugin
 * 
 * uniform_gui.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

void
uniform_gui (void)
{
  GtkRequisition req;

  /* stdev label */
  uniform_stdev_label = gtk_label_new (NULL);
  gtk_widget_set_tooltip_text (uniform_stdev_label, TT_UNIFORM_STDEV);
  gtk_widget_show (uniform_stdev_label);
  uniform_stdev_align = gtk_alignment_new (0.0, 0.0, 0.0, 0.0);
  gtk_container_add (GTK_CONTAINER (uniform_stdev_align),
		     uniform_stdev_label);
  gtk_widget_show (uniform_stdev_align);

  /* width */
  uniform_width_hbox = gtk_hbox_new (FALSE, 10);
  uniform_width_label = gtk_label_new (_("Width"));
  uniform_width_adj =
    gtk_adjustment_new (settings.uniform_width, 0, 170, 1, 1, 0);
  g_signal_connect_swapped (uniform_width_adj, "value-changed",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  g_signal_connect (uniform_width_adj, "value-changed",
		    G_CALLBACK (set_uniform_width), NULL);
  set_uniform_width (GTK_ADJUSTMENT (uniform_width_adj), NULL);
  uniform_width_scale = gtk_hscale_new (GTK_ADJUSTMENT (uniform_width_adj));
  gtk_scale_set_draw_value (GTK_SCALE (uniform_width_scale), FALSE);
  gtk_widget_size_request (uniform_width_scale, &req);
  gtk_widget_set_size_request (uniform_width_scale, req.width * 3,
			       req.height);
  uniform_width_spin =
    gtk_spin_button_new (GTK_ADJUSTMENT (uniform_width_adj), 1, 0);
  gtk_widget_set_tooltip_text (uniform_width_label, TT_UNIFORM_WIDTH);
  gtk_widget_set_tooltip_text (uniform_width_spin, TT_UNIFORM_WIDTH);
  gtk_widget_set_tooltip_text (uniform_width_scale, TT_UNIFORM_WIDTH);
  gtk_box_pack_start (GTK_BOX (uniform_width_hbox), uniform_width_label,
		      FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (uniform_width_hbox), uniform_width_scale, TRUE,
		      TRUE, 0);
  gtk_box_pack_start (GTK_BOX (uniform_width_hbox), uniform_width_spin, FALSE,
		      FALSE, 0);
  gtk_widget_show (uniform_width_hbox);
  gtk_widget_show (uniform_width_label);
  gtk_widget_show (uniform_width_scale);
  gtk_widget_show (uniform_width_spin);

  notebook_box[2] = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (notebook_box[2]), 10);
  gtk_box_pack_start (GTK_BOX (notebook_box[2]), uniform_width_hbox, FALSE,
		      FALSE, 0);
  gtk_box_pack_start (GTK_BOX (notebook_box[2]), uniform_stdev_align, FALSE,
		      FALSE, 0);
  gtk_widget_show (notebook_box[2]);
}
