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
  //    GtkTextMark *mark;
  //    GtkTextIter iter;
  const gchar *strcombo, *strkey, *strAuth, *strEditor, *strTitle,
	*strPublisher, *strVol, *strIss, *strPage, *strMon, *strNote,
	*strKey, *strYear, *strAdd, *strEdition,
	*strJournal, *strSeries, *strBooktitle, *strOrg,
	*strChap, *strSchool;

  strcombo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(e->combo));
  strkey = gtk_entry_get_text(GTK_ENTRY(e->entry1));
  strAuth = gtk_entry_get_text(GTK_ENTRY(e->entryAuth));
  strEditor = gtk_entry_get_text(GTK_ENTRY(e->entryEditor));
  strTitle = gtk_entry_get_text(GTK_ENTRY(e->entryTitle));
  strPublisher = gtk_entry_get_text(GTK_ENTRY(e->entryPublisher));
  strVol = gtk_entry_get_text(GTK_ENTRY(e->entryVol));
  strIss = gtk_entry_get_text(GTK_ENTRY(e->entryNumber));
  strPage = gtk_entry_get_text(GTK_ENTRY(e->entryPage));
  strMon = gtk_entry_get_text(GTK_ENTRY(e->entryMon));
  strNote = gtk_entry_get_text(GTK_ENTRY(e->entryNote));
  strKey = gtk_entry_get_text(GTK_ENTRY(e->entryKey));
  strYear = gtk_entry_get_text(GTK_ENTRY(e->entryYear));
  strAdd = gtk_entry_get_text(GTK_ENTRY(e->entryAddress));
  strEdition = gtk_entry_get_text(GTK_ENTRY(e->entryEdition));
  strJournal = gtk_entry_get_text(GTK_ENTRY(e->entryJournal));
  strSeries = gtk_entry_get_text(GTK_ENTRY(e->entrySeries));
  strBooktitle = gtk_entry_get_text(GTK_ENTRY(e->entryBooktitle));
  strOrg = gtk_entry_get_text(GTK_ENTRY(e->entryOrg));
  strSchool = gtk_entry_get_text(GTK_ENTRY(e->entrySchool));
  strChap = gtk_entry_get_text(GTK_ENTRY(e->entryChap));


  if(strcmp (strcombo,"==Entry Type==")==0){
    char 	*err_entry="Select Entry Type";
    caution(err_entry);
  }
  else if(strcmp(strkey,"")==0){
    char *err_key="Select Key";
    caution(err_key);
  }
  else{
    gtk_list_store_append(store, &siter);
    gtk_list_store_set(store, &siter,
	COL_BIB_KEY, 		strkey,
	COL_BIB_TYPE, 		strcombo,
	COL_BIB_AUTHOR, 	strAuth,
	COL_BIB_YEAR, 		strYear,
	COL_BIB_TITLE, 		strTitle,
	COL_BIB_JOURNAL,	strJournal,
	-1);

    GString *tstring = g_string_new ("");
    GString *str=g_string_new(buffer);
//    g_free(buffer);
    gtk_label_set_text(GTK_LABEL(flabel), filename);
    g_string_append_printf (tstring, "@%s{%s,\n",strcombo, strkey);
    if( strlen(strAuth)!=0)
      g_string_append_printf (tstring,"\tAuthor=\"%s\",\n", strAuth );
    if( strlen(strEditor)!=0)
      g_string_append_printf (tstring,"\tEditor=\"%s\",\n", strEditor);
    if( strlen(strTitle)!=0)
      g_string_append_printf (tstring, "\tTitle=\"%s\",\n", strTitle);
    if( strlen(strPublisher)!=0)
      g_string_append_printf (tstring, "\tPublisher=\"%s\",\n", strPublisher);
    if( strlen(strVol)!=0)
      g_string_append_printf (tstring, "\tVolume=\"%s\",\n", strVol);
    if( strlen(strIss)!=0)
      g_string_append_printf (tstring, "\tNumber=\"%s\",\n", strIss);
    if( strlen(strPage)!=0)
      g_string_append_printf (tstring, "\tPage=\"%s\",\n", strPage);
    if( strlen(strMon)!=0)
      g_string_append_printf (tstring, "\tMonth=\"%s\",\n", strMon);
    if( strlen(strNote)!=0)
      g_string_append_printf (tstring, "\tNote=\"%s\",\n", strNote);
    if( strlen(strKey)!=0)
      g_string_append_printf (tstring, "\tKey=\"%s\",\n", strKey);
    if( strlen(strYear)!=0)
      g_string_append_printf (tstring, "\tYear=\"%s\",\n", strYear);
    if( strlen(strAdd)!=0)
      g_string_append_printf (tstring, "\tAddress=\"%s\",\n", strAdd);
    if( strlen(strEdition)!=0)
      g_string_append_printf (tstring, "\tEdition=\"%s\",\n", strEdition);
    if( strlen(strJournal)!=0)
      g_string_append_printf (tstring, "\tJournal=\"%s\",\n", strJournal);
    if( strlen(strSeries)!=0)
      g_string_append_printf (tstring, "\tSeries=\"%s\",\n", strSeries);
    if( strlen(strBooktitle)!=0)
      g_string_append_printf (tstring, "\tBooktitle=\"%s\",\n", strBooktitle);
    if( strlen(strOrg)!=0)
      g_string_append_printf (tstring, "\tOrganization=\"%s\",\n", strOrg);
    if( strlen(strChap)!=0)
      g_string_append_printf (tstring, "\tChapter=\"%s\",\n", strChap);
    if( strlen(strSchool)!=0)
      g_string_append_printf (tstring, "\tSchool=\"%s\",\n", strSchool);
    g_string_append_printf (tstring,"}\n");
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

