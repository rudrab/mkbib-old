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
  //  g_print(gs_text);
  GScanner* gs_scan = g_scanner_new (NULL);
  g_scanner_input_text (gs_scan, gs_text, gs_BUFF);

  gs_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
  do {
    g_hash_table_remove_all (gs_table);
    parse_entry (gs_scan, gs_table);
    output_entry (gs_table);
    g_scanner_peek_next_token (gs_scan);
  }
  while (gs_scan->next_token != G_TOKEN_EOF &&
      gs_scan->next_token != G_TOKEN_ERROR);

  gs_tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  for (valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &siter);
      valid;
      valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &siter)) {
    char *string0, *string1, *string2, *string3, *string4,*string5, *string6, 
	 *string7, *string8, *string9,*string10, *string11, *string12, *string13, 
	 *string14,*string15, *string16, *string17, *string18, *string19;

    gtk_tree_model_get (GTK_TREE_MODEL (store), &siter,
	0, &string0, 	1, &string1, 	2, &string2, 	3, &string3,
	4, &string4, 	5, &string5, 	6, &string6, 	7, &string7,
	8, &string8, 	9, &string9, 	10, &string10,	11, &string11,
	12, &string12,  13, &string13,	14, &string14,	15, &string15,
	16, &string16,	17, &string17,	18, &string18,	19, &string19,
	-1);
    g_string_append_printf (ustring, "@%s{%s,\n",string1, string0);
    if( strlen(string2)!=0 ||string2!=NULL)
      g_string_append_printf (ustring,"\tAuthor=\"%s\",\n", string2);
    if( strlen(string3)!=0 ||string3!=NULL)
      g_string_append_printf (ustring,"\tYear=\"%s\",\n", string3);
    if( strlen(string4)!=0 ||string4!=NULL)
      g_string_append_printf (ustring, "\tTitle=\"%s\",\n", string4);
    if( (string5)!=0 || string5 !=NULL)
      g_string_append_printf (ustring, "\tPublisher=\"%s\",\n", string5);
    if( (string6)!=0 || string6 !=NULL)
      g_string_append_printf (ustring, "\tEditor=\"%s\",\n", string6);
    if( (string7)!=0 && string7 !=NULL)
      g_string_append_printf (ustring, "\tVolume=\"%s\",\n", string7);
    if( (string8)!=0  && string8 !=NULL)
      g_string_append_printf (ustring, "\tNumber=\"%s\",\n", string8);
    if( (string9)!=0 && string9 !=NULL) 
      g_string_append_printf (ustring, "\tPages=\"%s\",\n", string9);
    if( (string10)!=0 && string10 !=NULL)
      g_string_append_printf (ustring, "\tMonth=\"%s\",\n", string10);
    if( (string11)!=0 && string11 !=NULL)
      g_string_append_printf (ustring, "\tNote=\"%s\",\n", string11);
    if( (string12)!=0 && string12 !=NULL)
      g_string_append_printf (ustring, "\tKey=\"%s\",\n", string12);
    if( (string13)!=0 && string13 !=NULL)
      g_string_append_printf (ustring, "\tAddress=\"%s\",\n", string13);
    if( (string14)!=0 && string14 !=NULL)
      g_string_append_printf (ustring, "\tEdition=\"%s\",\n", string14);
    if( (string15)!=0 && string15 !=NULL)
      g_string_append_printf (ustring, "\tJournal=\"%s\",\n", string15);
    if( (string16)!=0 && string16 !=NULL)
      g_string_append_printf (ustring, "\tSeries=\"%s\",\n", string16);
    if( (string17)!=0 && string17 !=NULL)
      g_string_append_printf (ustring, "\tBooktitle=\"%s\",\n", string17);
    if( (string18)!=0 && string18 !=NULL)
      g_string_append_printf (ustring, "\tOrganization=\"%s\",\n", string18);
    if( (string19)!=0 && string19 !=NULL)
      g_string_append_printf (ustring, "\tChapter=\"%s\",\n", string19);
    g_string_append_printf (ustring,"}\n"); 
  }
  g_string_append(str,ustring->str);
  buffer=str->str;
  buf_mod=TRUE;
  gtk_widget_destroy(gtk_widget_get_toplevel (window));
}

void close_widget(GtkWidget *window, gpointer data){
}
