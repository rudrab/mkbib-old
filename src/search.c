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
#include <string.h>
#include <libsoup/soup.h>
#include "main.h"
#define gs_BUFF 1024
GtkWidget* gs_txt;

void search_gs(GtkWidget *widget, gpointer data) {
  const gchar *strAuth, *strTitle, *strYear;
  gchar *encoded, strurl[1024];
  Entries *e = (Entries*) data;
  strAuth = gtk_entry_get_text(GTK_ENTRY(e->entryAuth));
  strTitle = gtk_entry_get_text(GTK_ENTRY(e->entryTitle));
  strYear = gtk_entry_get_text(GTK_ENTRY(e->entryYear));

  if ((strlen(strTitle)!=0)&(strlen(strAuth)!=0)){
    encoded = soup_form_encode ("as_q", strTitle,
	"as_sauthors", strAuth,
	"as_ylo", strYear, NULL);
  }
  else if((strlen(strTitle)==0)&(strlen(strAuth)!=0)){
    encoded=soup_form_encode("as_q",strAuth,
	"as_ylo",strYear,NULL);
  }
  else if((strlen(strTitle)!=0)&(strlen(strAuth)==0)){
    encoded=soup_form_encode("as_q",strTitle,
	"as_ylo",strYear,NULL);
  }
  else if((strlen(strTitle)==0)&(strlen(strAuth)==0)){
    caution("You haven't given any search criteria!");
    return;
  }
  strcpy(strurl,"http://scholar.google.com/scholar?");
  strcat(strurl,encoded);

  gtk_show_uri(gdk_screen_get_default(),strurl,GDK_CURRENT_TIME,NULL);
  gtk_entry_set_text(GTK_ENTRY(e->entryAuth),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryYear),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryTitle),"");
}

void gen_gs(GtkWidget *widget, gpointer data) {
  void gs_open(GtkWidget *widget, gpointer data);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_title(GTK_WINDOW(window), "Enter a bibtex entry");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  GtkWidget* gen_grid=gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), gen_grid);

  gs_txt=gtk_text_view_new();
  gtk_widget_set_size_request(gs_txt, 250, 250);

  GtkWidget* con_gen= gtk_button_new_with_label("Create");
  g_signal_connect(con_gen, "clicked", G_CALLBACK(gs_open), NULL);
  //  g_signal_connect(con_gen, "clicked", G_CALLBACK(close_widget), NULL);

  GtkWidget *con_can= gtk_button_new_with_label("Cancel");
  g_signal_connect_swapped(con_can, "clicked", G_CALLBACK(gtk_widget_destroy), window);
  gtk_grid_attach(GTK_GRID(gen_grid), gs_txt,  0, 1, 10, 10);
  gtk_grid_attach(GTK_GRID(gen_grid), con_gen, 0, 12, 5, 1);
  gtk_grid_attach(GTK_GRID(gen_grid), con_can, 5, 12, 5, 1);
  gtk_widget_show_all(window);
}

void gs_open(GtkWidget *window, gpointer data) {
  GScanner *gs_scanner;
  GHashTable *gs_table;
  GError* error=NULL;
  GtkTextIter start, end;
  GtkListStore *gs_store;
  GtkTreeIter siter;
  GtkWidget *gs_tree;
  gboolean valid;
  GString *ustring = g_string_new ("");
  GString *str=g_string_new(NULL);

  GtkTextBuffer *gs_buf=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gs_txt));
  gtk_text_buffer_get_start_iter (gs_buf, &start);
  gtk_text_buffer_get_end_iter (gs_buf, &end);
  gchar *gs_text = gtk_text_buffer_get_text (gs_buf, &start, &end, FALSE);

  if (buffer == NULL){
//    buffer = (char *) malloc(strlen(gs_text) + 1);
    buffer=g_strdup(gs_text);
  }
  else{
    gchar *t=buffer;
    buffer=g_strconcat(buffer,gs_text,NULL);
    g_free(t);
  }
  gtk_list_store_clear(store);
  output_entry ();
  buf_mod=TRUE;
  gtk_widget_destroy(gtk_widget_get_toplevel (window));
}

void close_widget(GtkWidget *window, gpointer data){
}
