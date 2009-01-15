/* 
 * Noise generator GIMP plugin
 * 
 * interface.c
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#include "plugin.h"
#include "interface.h"

gboolean
user_interface (GimpDrawable * drawable)
{
  gboolean run;

  gimp_ui_init (_("Noise generator"), FALSE);

  /* prepare the preview */
  preview = gimp_drawable_preview_new (drawable, &settings.show_preview);
  g_signal_connect_swapped (preview, "invalidated", G_CALLBACK (noise_gen),
			    drawable);
  GtkWidget *ctrls = gimp_preview_get_controls (GIMP_PREVIEW (preview));
  GList *list = gtk_container_get_children (GTK_CONTAINER (ctrls));
  preview_toggle = list->data;
  g_list_free (list);
  g_object_ref (preview_toggle);
  gtk_container_remove (GTK_CONTAINER (ctrls), preview_toggle);
  gtk_widget_show (preview);

  poisson_gui ();
  gauss_gui ();
  uniform_gui ();
  laplace_gui ();
  lorentz_gui ();

  /* prepare the notebook */
  notebook = gtk_notebook_new ();
  /* TRANSLATORS: this is the surname of the French mathematician
     Simeon-Denis Poisson */
  notebook_label[0] = gtk_label_new (_("Poisson"));
  gtk_widget_set_tooltip_text (notebook_label[0], TT_POISSON);
  /* TRANSLATORS: this is the surname of the German mathematician
     Carl-Friedrich Gauss */
  notebook_label[1] = gtk_label_new (_("Gauss"));
  gtk_widget_set_tooltip_text (notebook_label[1], TT_GAUSS);
  /* TRANSLATORS: Uniform as in uniform distribution. */
  notebook_label[2] = gtk_label_new (_("Uniform"));
  gtk_widget_set_tooltip_text (notebook_label[2], TT_UNIFORM);
  /* TRANSLATORS: this is the surname of the French mathematician
     Pierre-Simon Laplace */
  notebook_label[3] = gtk_label_new (_("Laplace"));
  gtk_widget_set_tooltip_text (notebook_label[3], TT_LAPLACE);
  /* TRANSLATORS: this is the surname of the Dutch physicist Hendrik Lorentz */
  notebook_label[4] = gtk_label_new (_("Lorentz"));
  gtk_widget_set_tooltip_text (notebook_label[4], TT_LORENTZ);
  gtk_widget_show (notebook_label[0]);
  gtk_widget_show (notebook_label[1]);
  gtk_widget_show (notebook_label[2]);
  gtk_widget_show (notebook_label[3]);
  gtk_widget_show (notebook_label[4]);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), notebook_box[0],
			    notebook_label[0]);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), notebook_box[1],
			    notebook_label[1]);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), notebook_box[2],
			    notebook_label[2]);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), notebook_box[3],
			    notebook_label[3]);
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), notebook_box[4],
			    notebook_label[4]);
  gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), settings.lastpage);
  g_signal_connect (notebook, "switch-page", G_CALLBACK (change_page), NULL);
  gtk_widget_show (notebook);

  /* prepare luminance select */
  if (channels > 2)
    {
      lum_toggle =
	gtk_check_button_new_with_label (_("Luminance noise only"));
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (lum_toggle),
				    settings.luminance_only);
      g_signal_connect (lum_toggle, "toggled", G_CALLBACK (set_ycbcr_mode),
			NULL);
      g_signal_connect_swapped (lum_toggle, "toggled",
				G_CALLBACK (gimp_preview_invalidate),
				preview);
      gtk_widget_set_tooltip_text (lum_toggle, TT_LUMINANCE);
      gtk_widget_show (lum_toggle);
    }
  else
    {
      lum_toggle = NULL;
    }

  /* gamma toggle */
  gamma_toggle = gtk_check_button_new_with_label (_("Gamma correction"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gamma_toggle),
				settings.correct_gamma);
  gtk_widget_set_tooltip_text (gamma_toggle, TT_GAMMA);
  g_signal_connect (gamma_toggle, "toggled", G_CALLBACK (set_gamma), NULL);
  g_signal_connect_swapped (gamma_toggle, "toggled",
			    G_CALLBACK (gimp_preview_invalidate), preview);
  gtk_widget_show (gamma_toggle);

  /* toggle vbox */
  toggle_vbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (toggle_vbox), lum_toggle, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (toggle_vbox), gamma_toggle, FALSE, FALSE, 0);
  gtk_widget_show (toggle_vbox);

  /* buttons */
  reset_button = gtk_button_new_from_stock (GIMP_STOCK_RESET);
  gtk_widget_set_tooltip_text (reset_button, TT_RESET);
  g_signal_connect (reset_button, "clicked", G_CALLBACK (reset), NULL);
  gtk_widget_show (reset_button);
  button_hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX (button_hbox), preview_toggle, TRUE, TRUE, 0);
  g_object_unref (preview_toggle);
  gtk_box_pack_start (GTK_BOX (button_hbox), reset_button, FALSE, FALSE, 0);
  gtk_widget_show (button_hbox);

  /* prepare the dialog vbox */
  dialog_vbox = gtk_vbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), notebook, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), toggle_vbox, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), button_hbox, FALSE, FALSE, 0);
  gtk_widget_show (dialog_vbox);

  /* prepare the dialog hbox */
  dialog_hbox = gtk_hbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_hbox), 10);
  gtk_box_pack_start (GTK_BOX (dialog_hbox), preview, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (dialog_hbox), dialog_vbox, TRUE, TRUE, 0);
  gtk_widget_show (dialog_hbox);

  /* prepeare the dialog */
  dialog = gimp_dialog_new (_("Noise generator"), "noise generator", NULL, 0,
			    gimp_standard_help_func,
			    GIMP_PLUGIN_ID, GTK_STOCK_CANCEL,
			    GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
			    GTK_RESPONSE_OK, NULL);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), dialog_hbox);
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  if (settings.winxsize > 0 && settings.winysize > 0)
    gtk_window_resize (GTK_WINDOW (dialog), settings.winxsize,
		       settings.winysize);
  gtk_widget_show (dialog);

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_window_get_size (GTK_WINDOW (dialog), &(settings.winxsize),
		       &(settings.winysize));

  /* FIXME: destroy all widgets - memory leak! */
  gtk_widget_destroy (dialog);

  return run;
}
