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

#include <string.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include "main.h"

GtkWidget *window,
	  *flabel,
	  *tree,
	  *bibcombo;
char *filename,
     *key, 
     *val;
GtkListStore *store;
GtkTreeIter siter;
#define slen 64
#define CHAR_BUFF 5000

/*
 *Saving in the same file
 */
void save_file(GtkWidget *widget, gpointer data){
  GError *Err;
  g_file_set_contents(filename, buffer, -1, &Err);
  if (Err)
  {      caution("Failed to save in %s",filename);
//      g_free(str); 
      g_error_free(Err);
    }
  g_print("%s",filename);
  g_free(buffer);
  buf_mod=FALSE;
}

/*
 *Create and Save in a new/existing file
 * The Save_As function
 */
void save_file_as(GtkWidget *widget, gpointer data)
{
  gchar *fname=NULL, *str; 
  gchar *cts;
  GError *Err=NULL;
  GtkWidget *dialog;
  gsize length;
  dialog=gtk_file_chooser_dialog_new("Save File As", GTK_WINDOW(window),
      GTK_FILE_CHOOSER_ACTION_SAVE, 
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 
      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    g_file_set_contents(filename, buffer, -1, &Err);
    g_free(buffer);
    if (Err)
    {
      caution("Failed to save in %s",filename);
      g_free(str); 
      g_error_free(Err);
    }
  }
  buf_mod=FALSE;
  gtk_widget_destroy(dialog);
}


void new_file(GtkWidget *widget, gpointer data) 
{
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window),
      GTK_FILE_CHOOSER_ACTION_SAVE,
      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),"Untitled.bib");
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) 
  {
    filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    if(TRUE)
    {
      remove(filename);
      char* markup=g_markup_printf_escaped ("<span style=\"italic\">%s</span>", filename);
      gtk_label_set_markup(GTK_LABEL(flabel), markup);
      gtk_widget_destroy(dialog);
    }
    FILE *fn=g_fopen(filename,"w");
    g_fprintf(fn,"#This File is Created by mkBIB\n");
    fclose(fn);
  }
  gtk_widget_destroy(dialog);
}

void open_file(GtkWidget *widget, gpointer data)
{
  GScanner *scanner;
  GHashTable *table;
  GError* error=NULL;
  GtkWidget *dialog;
  GtkFileFilter *filter;
  dialog = gtk_file_chooser_dialog_new("Open File", NULL,
      GTK_FILE_CHOOSER_ACTION_OPEN,
      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      NULL);
  filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "All files (*.*)");
  gtk_file_filter_add_pattern(filter, "*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "Bibtex file (*.bib)");
  gtk_file_filter_add_pattern(filter, "*.bib");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_list_store_clear (store);
    g_file_get_contents(filename, &buffer, &length , &error);
    g_assert(!error);
    buf_mod=FALSE;
//    GtkTextBuffer* buftext;
//    gtk_text_buffer_set_text(buftext, buffer, &length);

    char* markup=g_markup_printf_escaped ("<span style=\"italic\">%s</span>", filename);
    gtk_label_set_markup(GTK_LABEL(flabel), markup);
    gtk_widget_destroy(dialog);

    scanner = g_scanner_new (NULL);
    g_scanner_input_text (scanner, buffer, CHAR_BUFF);

    table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
    do {
      g_hash_table_remove_all (table);
      parse_entry (scanner, table);
      output_entry (table);
      g_scanner_peek_next_token (scanner);
    }
    while (scanner->next_token != G_TOKEN_EOF &&
	scanner->next_token != G_TOKEN_ERROR);


    /* finish parsing */
    g_scanner_destroy (scanner);
    g_hash_table_destroy (table);
  }
  else{
    gtk_widget_destroy(dialog);
    //    g_free(buffer);
  }
}

/*void mk_bib(GtkWidget *widget, gpointer data) 
  {
  if (!filename) 
  {
  caution("No File Selected");
  } 
  else 
  {
  char *bibstyl = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(bibcombo));
  if (strcmp(bibstyl, "==select bibstyle==") == 0) 
  {
  caution("Select Bibliography style file");
  }
  else {
  char *p1 = filename;
  char *p2 = rindex(p1, '.');
  if (p2 != NULL) {
  if (strcmp(p2, ".bib") == 0) {
 *p2 = '\0';
 }
 }
 char *basefn = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(bibcombo));
 strcat(basefn, ".tex");
 FILE *texout = fopen(basefn, "w");
 g_fprintf(texout, "\\documentclass[a4paper]{article}\n"
 "\\begin{document}\n"
 "\t\\nocite{*}\n"
 "\t\\bibliography{%s}\n"
 "\t\\bibliographystyle{%s}\n"
 "\\end{document}",
 p1, bibstyl);
 fclose(texout);
 }
 }
 }
 */

