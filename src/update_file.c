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
#include <glib.h>
#include <string.h>
/* #include <stdio.h>
 * #include <stdlib.h>
 * #include <sys/types.h>
 * #include <sys/stat.h>
 * #include <gdk/gdk.h>
 * #include <gdk/gdkkeysyms.h>
 */
#include "main.h"
GtkWidget *flabel;
char *filename;
void caution();
GtkListStore *store;
GtkTreeIter siter;
GError* error=NULL;


void activate_func(GtkWidget *widget, gpointer data) {
  Entries *e = (Entries*) data;
  const gchar *strings[21]={""};
/*  strings[0]  = g_malloc(strlen(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(e->combo))));
  strings[1]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entry1))));
  strings[2]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryAuth))));
  strings[3]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryEditor))));
  strings[4]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryTitle))));
  strings[5]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryPublisher))));
  strings[6]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryVol))));
  strings[7]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryNumber))));
  strings[8]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryPage))));
  strings[9]  = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryMon))));
  strings[10] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryNote))));
  strings[11] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryKey))));
  strings[12] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryYear))));
  strings[13] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryAddress))));
  strings[14] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryEdition))));
  strings[15] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryJournal))));
  strings[16] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entrySeries))));
  strings[17] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryBooktitle))));
  strings[18] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryOrg))));
  strings[19] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entrySchool))));
  strings[20] = g_malloc(strlen(gtk_entry_get_text(GTK_ENTRY(e->entryChap))));
*/
  strings[0]  = g_strdup(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(e->combo)));
  strings[1]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entry1)));
  strings[2]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryAuth)));
  strings[3]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryEditor)));
  strings[4]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryTitle)));
  strings[5]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryPublisher)));
  strings[6]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryVol)));
  strings[7]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryNumber)));
  strings[8]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryPage)));
  strings[9]  = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryMon)));
  strings[10] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryNote)));
  strings[11] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryKey)));
  strings[12] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryYear)));
  strings[13] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryAddress)));
  strings[14] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryEdition)));
  strings[15] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryJournal)));
  strings[16] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entrySeries)));
  strings[17] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryBooktitle)));
  strings[18] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryOrg)));
  strings[19] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entrySchool)));
  strings[20] = g_strdup(gtk_entry_get_text(GTK_ENTRY(e->entryChap)));
 

  if(strcmp (strings[0],"==Entry Type==")==0){
    caution("Select Entry Type");
  }
  else if(strcmp(strings[1],"")==0){
    caution("Select Key");
  }
  else{
    gtk_list_store_append(store, &siter);
    gtk_list_store_set(store, &siter,
	COL_BIB_KEY, 		strings[0],
	COL_BIB_TYPE, 		strings[1],
	COL_BIB_AUTHOR, 	strings[2],
	COL_BIB_YEAR, 		strings[12],
	COL_BIB_TITLE, 		strings[4],
	COL_BIB_JOURNAL,	strings[5],
	-1);

    GString *tstring = g_string_new ("");
    GString *str=g_string_new(buffer);
    gtk_label_set_text(GTK_LABEL(flabel), filename);
    g_string_append_printf (tstring, "@%s{%s",strings[0], strings[1]);
    gchar *keyword[21] = {
      "Type"        , "ID",
      "Author" 	    , "Editor" , "Title" , "Publisher",
      "Volume"      , "Number" , "Pages"  , "Month"    ,
      "Note"        , "Key"    , "Year"  , "Address"  ,
      "Edition"     , "Journal", "Series", "Booktitle",
      "Organization", "Chapter", "School"};

    int i=2;
    while (i < 21) {
      if (strlen(strings[i]) != 0) {
	g_string_append_printf(tstring, ",\n\t%s=\"%s\"",
	    keyword[i], strings[i]);
      g_free((char*)strings[i]);
      strings[i]=NULL;
      g_print("Cleaned:%d\n",i);
      } 
      i++;
    }

    g_string_append_printf (tstring,"\n}\n");
    g_string_append(str,tstring->str);
    g_string_free(tstring, TRUE);
    buffer=str->str;
    buf_mod=TRUE;
  }
  gtk_entry_set_text(GTK_ENTRY(e->entry1),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryAuth),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryEditor),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryTitle),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryPublisher),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryVol),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryNumber),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryPage),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryMon),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryNote),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryKey),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryYear),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryAddress),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryEdition),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryJournal),"");
  gtk_entry_set_text(GTK_ENTRY(e->entrySeries),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryBooktitle),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryOrg),"");
  gtk_entry_set_text(GTK_ENTRY(e->entryChap),"");
  gtk_entry_set_text(GTK_ENTRY(e->entrySchool),"");
}

