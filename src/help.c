/* Copyright (C) 2012 Rudra Banerjee*/

/*
   This file is part of mkbib.

   mkbib is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   mkbib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with mkbib.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include <glib/gi18n.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <gtk/gtk.h>
#include <string.h>

GtkWidget *window;
void help_about (GtkMenuItem *helpabout, GtkWindow *window)
{
  const gchar *auth[]={ "Rudra Banerjee <rudra.banerjee@aol.co.uk>", NULL };
  const gchar trans[]={"Kathakali Banerjee <bnrj.kathakali@hotmail.co.uk>"};
  gchar *hstr=g_strdup_printf("%s/mkbib.svg", PIXMAP);
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(hstr, NULL);

  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "mkbib");
  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), auth);
  gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(dialog), trans);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), VERSION); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
      "Copyright 2012 ©Rudra Banerjee"
      );
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
      "This is a simple BibTex manager  \n"
      "which can be used to create and edit standard bibtex file."
      );
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), 
      "http://www.nongnu.org/mkbib/"
      );
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog),
      "  This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version. \n This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. \n  You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., \n59 Temple Place, Suite 330, Boston, MA  02111-1307  USA"
      );    
  gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(dialog),TRUE);
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
void help_content(GtkWidget *widget)
{
  GError *error = NULL;
  gtk_show_uri (gtk_widget_get_screen (widget),
      "help:mkbib",
      gtk_get_current_event_time(),
      &error);

  if(error)
  {
    g_printf ( "%s \n", error->message);
    g_error_free (error);
  } 
}


