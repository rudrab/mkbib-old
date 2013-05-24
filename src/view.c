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
char *filename,*key, *val;
GtkTreeIter siter;
#define slen 1064

void activate_func(GtkWidget *widget, gpointer data);

void output_entry (gpointer data) {
  extern  FILE* yyin;
  extern int yyparse (void);
  yyin=fmemopen(buffer,strlen(buffer),"r");
  return yyparse();
}


GtkWidget *create_view_and_model(void) {
  GtkCellRenderer *cell;
  void
    cell_edited(GtkCellRendererText *renderer,
	gchar *path,
	gchar *new_text,
	GtkTreeView *treeview);
  GtkTreeViewColumn *col_key,*col_year,*col_type,*col_auth,*col_title,*col_journal;

  store = gtk_list_store_new (NUM_COLS, 
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);

  /* #1: KEY COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  g_object_set (cell,
      "editable", TRUE,
      NULL);

  g_signal_connect (cell, 
      "edited",G_CALLBACK(cell_edited), 
      tree);

  g_object_set_data (G_OBJECT (cell), 
      "column", GINT_TO_POINTER (COL_BIB_KEY));
  col_key=gtk_tree_view_column_new_with_attributes (
      "Key", cell,
      "text", COL_BIB_KEY,
      NULL);
  gtk_tree_view_column_set_sort_column_id( col_key, ID_KEY);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_key);
  gtk_tree_view_column_set_max_width  (col_key,100);


  /* #2: TYPE COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  g_object_set(G_OBJECT(cell), "wrap-mode", PANGO_WRAP_WORD, 
      "wrap-width",100, NULL);
  col_type=gtk_tree_view_column_new_with_attributes (
      "Type", cell,
      "text", COL_BIB_TYPE,
      NULL);
  gtk_tree_view_column_set_sort_column_id( col_type, ID_TYPE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_type);



  /* #3: AUTHOR COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  g_object_set(G_OBJECT(cell), "wrap-mode", PANGO_WRAP_WORD, 
      "wrap-width",300, NULL);
  col_auth=gtk_tree_view_column_new_with_attributes (
      "Author", cell,
      "text", COL_BIB_AUTHOR,
      NULL);
  gtk_tree_view_column_set_sort_column_id( col_auth, ID_AUTHOR);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_auth);
  //  gtk_tree_view_column_set_max_width  (col_auth,350);


  /* #4: YEAR COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  col_year=gtk_tree_view_column_new_with_attributes (
      "Year", cell,
      "text", COL_BIB_YEAR,
      NULL);
  gtk_tree_view_column_set_sort_column_id( col_year, ID_YEAR);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_year);
  gtk_tree_view_column_set_max_width  (col_year,65);


  /* #5: TITLE COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  g_object_set(G_OBJECT(cell), "wrap-mode", PANGO_WRAP_WORD, 
      "wrap-width",300, NULL);
  col_title=gtk_tree_view_column_new_with_attributes (
      "Title", cell,
      "text", COL_BIB_TITLE,
      NULL);
  gtk_tree_view_column_set_sort_column_id( col_title, ID_TITLE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_title);
  //  gtk_tree_view_column_set_max_width  (col_pub,350);


  /* #6: Journal COLUMN */  
  cell = gtk_cell_renderer_text_new ();
  g_object_set(G_OBJECT(cell), "wrap-mode", PANGO_WRAP_WORD, 
      "wrap-width",120, NULL);
  col_journal=gtk_tree_view_column_new_with_attributes (
      "Journal", cell,
      "text", COL_BIB_JOURNAL,
      NULL);
  gtk_tree_view_column_set_sort_column_id(col_journal, ID_JOURNAL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), col_journal);

  return tree;
}


/* Apply the changed text to the cell if it is not an empty string. */
void cell_edited(GtkCellRendererText *renderer,
    gchar *path,
    gchar *new_text,
    GtkTreeView *treeview)
{
  guint column;
  GtkTreeIter iter;
  gboolean valid;
  GtkTreeModel *model;
  GString *ustring = g_string_new ("");
  GString *str=g_string_new(NULL);

  //  g_free(buffer);
  gpointer columnptr = g_object_get_data(G_OBJECT(renderer), "column");
  column = GPOINTER_TO_UINT(columnptr);

  if (g_ascii_strcasecmp (new_text, "") != 0)
  {
    model = gtk_tree_view_get_model (treeview);
    if (gtk_tree_model_get_iter_from_string (model, &iter, path))
      gtk_list_store_set (GTK_LIST_STORE (model), &iter, column, new_text, -1);
  }
  for (valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter);
      valid;
      valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (store), &iter)) {
    char *string0, *string1, *string2, *string3, *string4,*string5, *string6, 
	 *string7, *string8, *string9,*string10, *string11, *string12, *string13, 
	 *string14,*string15, *string16, *string17, *string18, *string19;

    gtk_tree_model_get (GTK_TREE_MODEL (store), &iter,
	0, &string0, 	1, &string1, 	2, &string2, 	3, &string3,
	4, &string4, 	5, &string5, 	6, &string6, 	7, &string7,
	8, &string8, 	9, &string9, 	10, &string10,	11, &string11,
	12, &string12,  13, &string13,	14, &string14,	15, &string15,
	16, &string16,	17, &string17,	18, &string18,	19, &string19,
	-1);//
    /*    g_print("0:%s\n1:%s\n2:%s\n3:%s\n4:%s\n5:%s\n6:%s\n7:%s\n8:%s\n9:%s\n10:%s\n11:%s\n12:%s\n13:%s\n14:%s\n15:%s\n16:%s\n17:%s\n18:%s\n19:%s",
	  string0, string1, string2, string3, string4, string5, string6,
	  string7, string8, string9,string10, string11, string12, string13,
	  string14,string15, string16, string17, string18, string19);
	  */
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
      g_string_append_printf (ustring, "\tJournal=\"%s\",\n", string14);
    if( (string15)!=0 && string15 !=NULL)
      g_string_append_printf (ustring, "\tEdition=\"%s\",\n", string15);
    if( (string16)!=0 && string16 !=NULL)
      g_string_append_printf (ustring, "\tSeries=\"%s\",\n", string16);
    if( (string17)!=0 && string17 !=NULL)
      g_string_append_printf (ustring, "\tBooktitle=\"%s\",\n", string17);
    if( (string18)!=0 && string18 !=NULL)
      g_string_append_printf (ustring, "\tOrganization=\"%s\",\n", string18);
    if( (string19)!=0 && string19 !=NULL)
      g_string_append_printf (ustring, "\tChapter=\"%s\",\n", string19);

    g_string_append_printf(ustring, "%s\n","}");

    g_free (string0);   g_free (string1);
    g_free (string2);   g_free (string3);
    g_free (string4);   g_free (string5);
    g_free (string6);   g_free (string7);
    g_free (string8);   g_free (string9);
    g_free (string10);  g_free (string11);
    g_free (string12);  g_free (string13);
    g_free (string14);  g_free (string15);
    g_free (string16);  g_free (string17);
    g_free (string18);  g_free (string19);
  }
  g_string_append(str,ustring->str);
  buffer=str->str;
  buf_mod=TRUE;
}
