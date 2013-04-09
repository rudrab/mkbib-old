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

#include <gtk/gtk.h>
#include <stdio.h>
GtkWidget *window;

void caution(char *error){
  GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window),
      GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_MESSAGE_ERROR,
      GTK_BUTTONS_CLOSE,
      error,
      NULL, NULL);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
