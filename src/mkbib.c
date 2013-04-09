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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glib/gi18n.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include "main.h"

FILE *tauth,*tyear,*ttitle;
GtkWidget *window,
	  *menubar,
	  *flabel,
	  *tree,
	  *dialog,
	  *image,
	  *bibcombo,
	  *entrypdf;
char *filename,*key, *val;
GtkAccelGroup *menuGroup;
GtkListStore *store;
GtkTreeIter siter;

void help_about(GtkMenuItem *helpabout, GtkWindow *parentWindow);
void activate_func(GtkWidget *widget, gpointer data);
void search_gs(GtkWidget *widget, gpointer data);
void output_entry (GHashTable *table);
guint parse_entry (GScanner   *scanner,GHashTable *table);
GtkWidget *create_view_and_model(void);
void new_file(GtkWidget *widget, gpointer data);
void open_file(GtkWidget *widget, gpointer data);
void save_file(GtkWidget *widget, gpointer data);
void save_file_as(GtkWidget *widget, gpointer data);
void mk_bib(GtkWidget *widget, gpointer data);
void caution();
void help_content(GtkWidget *widget, gpointer data);
void gen_gs(GtkWidget *widget, gpointer data);

GdkPixbuf *create_pixbuf(const gchar * iconname) {
  GdkPixbuf *pixbuf;
  GError *error = NULL;
  pixbuf = gdk_pixbuf_new_from_file(iconname, &error);
  if (!pixbuf) {
    fprintf(stderr, "%s\n", error->message);
    g_error_free(error);
  }

  return pixbuf;
}

gint sort_by_author(GtkTreeModel *model,
    GtkTreeIter *a,
    GtkTreeIter *b,
    gpointer userdata)
{
  gchar *first,*second ;
  gtk_tree_model_get( model , a, COL_BIB_AUTHOR , &first,-1);
  gtk_tree_model_get( model , b, COL_BIB_AUTHOR , &second, -1);

  gint return_value = g_utf8_collate ( first , second ) ;
  g_free ( first) ;
  g_free ( second ) ;
  return return_value ;
}

void main_window_quit(GtkWidget *widget, gpointer data){
  if(!buf_mod){
    gtk_main_quit();
  }
  else{
    gboolean ret = FALSE;
    GtkWidget *dialog = gtk_message_dialog_new (NULL,
      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_MESSAGE_QUESTION,
      GTK_BUTTONS_YES_NO,
      "Do you want to save the changes before quit?");

  gtk_window_set_title (GTK_WINDOW (dialog), "Save?");
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_NO)
  {
    gtk_main_quit();
  }      
  else {
    save_file_as(widget,data);
    gtk_main_quit();
  }
  gtk_widget_destroy (dialog);      
  }     
}

int main(int argc,
    char *argv[]) {
  GtkWidget *grid, 
  *button, 
  *frame,
  *notebook, 
  *label,
  *scrolledwindow, 
  *filemenu, 
  *editmenu,
  *helpmenu,
  *file, 
  *new, 
  *open, 
  *save, 
  *save_as, 
  *quit,
  *edit, 
  *edit_entry, 
  *help, 
  *about, 
  *sep;
  GtkAccelGroup *accel_group = NULL;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "mkBIB");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gchar *sicon=g_strdup_printf("%s/icon.svg",PIXMAP);
  gtk_window_set_icon(GTK_WINDOW(window), 
      create_pixbuf(sicon));
  g_free(sicon);
  g_signal_connect(window, "delete-event",
      G_CALLBACK(main_window_quit), NULL);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  menubar = gtk_menu_bar_new();
  filemenu = gtk_menu_new();
  editmenu = gtk_menu_new();
  helpmenu = gtk_menu_new();

  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  file = gtk_menu_item_new_with_mnemonic("_File");
  new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accel_group);
  open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, accel_group);
  save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_group);
  save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accel_group);
  sep = gtk_separator_menu_item_new();
  quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

  gtk_widget_add_accelerator (save, "activate", accel_group,
      GDK_KEY_s, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator (save_as, "activate", accel_group,
      GDK_KEY_S, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);


  edit = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
  edit_entry = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, accel_group);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), edit_entry);


  help = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
  about = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accel_group);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), about);

  GtkWidget *contents = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, accel_group);
  gtk_widget_add_accelerator(contents, "activate", accel_group, GDK_KEY_F1, 0, GTK_ACCEL_VISIBLE);    
  gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), contents);

  g_signal_connect(G_OBJECT(new), "activate", G_CALLBACK(new_file), NULL);
  g_signal_connect(G_OBJECT(open), "activate", G_CALLBACK(open_file), NULL);
  g_signal_connect(G_OBJECT(save), "activate", G_CALLBACK(save_file), NULL);
  g_signal_connect(G_OBJECT(save_as), "activate", G_CALLBACK(save_file_as), NULL);
  g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(main_window_quit), NULL);
  //    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(edit_entry), "activate", G_CALLBACK(gen_gs), NULL);

  g_signal_connect(G_OBJECT(contents), "activate", G_CALLBACK(help_content), NULL);
  g_signal_connect(G_OBJECT(about), "activate", G_CALLBACK(help_about), (gpointer) window);


  Entries *e = g_slice_new(Entries);


  e->combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "==Entry Type==");
  gtk_combo_box_set_active(GTK_COMBO_BOX(e->combo), 0);
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Book");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Article");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Booklet");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Conference");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "inBook");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "inCollection");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "inProceedings");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Manual");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "MastersThesis");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Misc");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "PhdThesis");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Proceedings");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Techreport");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(e->combo), "Unpublished");

  e->entry1 = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entry1), "bibKey");

  scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow), 
      GTK_SHADOW_IN);
  gtk_widget_set_size_request(scrolledwindow, 1000, 175);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
      GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  tree = create_view_and_model();
  gtk_container_add(GTK_CONTAINER(scrolledwindow), tree);

  notebook = gtk_notebook_new();
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);


  /*
   * Tab 1
   */
  frame = gtk_frame_new("");

  GtkWidget *page1 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(frame), page1);

  e->entryAuth = gtk_entry_new();
  GtkWidget *Authlabel = gtk_label_new("Author");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryAuth), "Author");

  e->entryEditor = gtk_entry_new();
  GtkWidget *Editorlabel = gtk_label_new("Editor");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryEditor), "Editor");

  e->entryTitle = gtk_entry_new();
  GtkWidget *Titlelabel = gtk_label_new("Title");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryTitle), "Title");

  e->entrySeries = gtk_entry_new();
  GtkWidget *Serieslabel = gtk_label_new("Series");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entrySeries), "Series");

  e->entryBooktitle = gtk_entry_new();
  GtkWidget *Booktitlelabel = gtk_label_new("Booktitle");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryBooktitle), "Booktitle");

  e->entryYear = gtk_entry_new();
  GtkWidget *Yearlabel = gtk_label_new("Year");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryYear), "Year");

  label = gtk_label_new("Authors/Title");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);

  gtk_grid_attach(GTK_GRID(page1), e->entryAuth, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Authlabel, 0, 0, 1, 1);

  gtk_grid_attach(GTK_GRID(page1), e->entryTitle, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Titlelabel, 0, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(page1), e->entryYear, 2, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Yearlabel, 0, 8, 1, 1);

  gtk_grid_attach(GTK_GRID(page1),gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),0,10,4,2);
  gtk_grid_attach(GTK_GRID(page1), e->entrySeries, 2, 12, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Serieslabel, 0, 12, 1, 1);

  gtk_grid_attach(GTK_GRID(page1), e->entryBooktitle, 2, 16, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Booktitlelabel, 0, 16, 1, 1);

  gtk_grid_attach(GTK_GRID(page1), e->entryEditor, 2, 20, 1, 1);
  gtk_grid_attach(GTK_GRID(page1), Editorlabel, 0, 20, 1, 1);


  /*
   * Tab 2
   */
  GtkWidget *frame2 = gtk_frame_new("");

  GtkWidget *page2 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(frame2), page2);

  e->entryJournal = gtk_entry_new();
  GtkWidget *Journallabel = gtk_label_new("Journal");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryJournal), "Journal");

  e->entryPublisher = gtk_entry_new();
  GtkWidget *Publabel = gtk_label_new("Publishers");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryPublisher), "Publisher");

  e->entryVol = gtk_entry_new();
  GtkWidget *Vollabel = gtk_label_new("Volume");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryVol), "Volume");

  e->entryNumber = gtk_entry_new();
  GtkWidget *Isslabel = gtk_label_new("Number");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryNumber), "Number");

  e->entryPage = gtk_entry_new();
  GtkWidget *Pagelabel = gtk_label_new("Page");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryPage), "Page");


  e->entryChap = gtk_entry_new();
  GtkWidget *Chaplabel = gtk_label_new("Chap");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryChap), "Chapter");

  label = gtk_label_new("Publishers");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame2, label);

  gtk_grid_attach(GTK_GRID(page2), e->entryJournal, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Journallabel, 0, 0, 1, 1);

  gtk_grid_attach(GTK_GRID(page2), e->entryPublisher, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Publabel, 0, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(page2), e->entryVol, 2, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Vollabel, 0, 8, 1, 1);

  gtk_grid_attach(GTK_GRID(page2), e->entryNumber, 2, 12, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Isslabel, 0, 12, 1, 1);

  gtk_grid_attach(GTK_GRID(page2), e->entryPage, 2, 16, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Pagelabel, 0, 16, 1, 1);

  gtk_grid_attach(GTK_GRID(page2), e->entryChap, 2, 20, 1, 1);
  gtk_grid_attach(GTK_GRID(page2), Chaplabel, 0, 20, 1, 1);

  /*
   * Tab 3
   */
  GtkWidget *frame3 = gtk_frame_new("");

  GtkWidget *page3 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(frame3), page3);

  e->entryMon = gtk_entry_new();
  GtkWidget *Monlabel = gtk_label_new("Month");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryMon), "Month");

  e->entryNote = gtk_entry_new();
  GtkWidget *Notelabel = gtk_label_new("Note");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryNote), "Note");

  e->entryKey = gtk_entry_new();
  GtkWidget *Keylabel = gtk_label_new("Key");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryKey), "Key");

  e->entryAddress = gtk_entry_new();
  GtkWidget *Addlabel = gtk_label_new("Address");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryAddress), "Address");

  e->entryEdition = gtk_entry_new();
  GtkWidget *Editionlabel = gtk_label_new("Edition");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryEdition), "Edition");

  e->entryOrg = gtk_entry_new();
  GtkWidget *Orglabel = gtk_label_new("Organisation");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entryOrg), "Organisation");

  e->entrySchool = gtk_entry_new();
  GtkWidget *Schoollabel = gtk_label_new("School");
  gtk_entry_set_placeholder_text(GTK_ENTRY(e->entrySchool), "School");

  label = gtk_label_new("Optional");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame3, label);


  gtk_grid_attach(GTK_GRID(page3), e->entryMon, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Monlabel, 0, 0, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entryNote, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Notelabel, 0, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entryKey, 2, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Keylabel, 0, 8, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entryAddress, 2, 12, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Addlabel, 0, 12, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entryEdition, 2, 16, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Editionlabel, 0, 16, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entryOrg, 2, 20, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Orglabel, 0, 20, 1, 1);

  gtk_grid_attach(GTK_GRID(page3), e->entrySchool, 2, 24, 1, 1);
  gtk_grid_attach(GTK_GRID(page3), Schoollabel, 0, 24, 1, 1);

  /*
   * Notebook Done
   */
  /*    bibcombo = gtk_combo_box_text_new();
#include "biblst"

gtk_combo_box_set_active(GTK_COMBO_BOX(bibcombo), 0);
g_signal_connect(G_OBJECT(bibcombo), "changed", G_CALLBACK(mk_bib), NULL);
gtk_grid_attach(GTK_GRID(grid), bibcombo, 31, 6, 2, 1);
*/
  button = gtk_button_new_with_label("CREATE");
  g_signal_connect(button, "clicked", G_CALLBACK(activate_func), e);

  GtkWidget *gen_q = gtk_button_new_with_label("Search Google Scholar");
  g_signal_connect(gen_q, "clicked", G_CALLBACK(search_gs), e);

  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  flabel = gtk_label_new("No file Selected");

  gtk_grid_attach(GTK_GRID(grid), flabel, 3, 0, 6, 1);
  gtk_grid_attach(GTK_GRID(grid), e->combo, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), e->entry1, 1, 1, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), notebook, 0, 3, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), scrolledwindow, 3, 1, 10, 7);
  gtk_grid_attach(GTK_GRID(grid), menubar, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gen_q, 1, 6, 2, 1);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
