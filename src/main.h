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

enum
{
  COL_BIB_KEY=0,
  COL_BIB_TYPE,		COL_BIB_AUTHOR,		COL_BIB_YEAR,  	COL_BIB_TITLE,
  COL_BIB_EDITOR,	COL_BIB_PUBLISHER,  	COL_BIB_VOL,  	COL_BIB_NUMBER,
  COL_BIB_PAGE,  	COL_BIB_MON,  		COL_BIB_NOTE,	COL_BIB_ADDRESS,
  COL_BIB_EDITION,  	COL_BIB_JOURNAL,	COL_BIB_SERIES, COL_BIB_BOOK,
  COL_BIB_ORG,		COL_BIB_CHAP,		COL_BIB_SCHOOL,	
  NUM_COLS} ;

typedef enum
{
  ID_KEY,  	ID_TYPE, 	ID_AUTHOR,	ID_YEAR,	ID_TITLE,
  ID_EDITOR,	ID_PUBLISHER,  	ID_VOL,  	ID_NUMBER,	ID_PAGE,  	
  ID_MON,	ID_NOTE,	ID_ADDRESS,	ID_EDITION,	ID_JOURNAL,
  ID_SERIES,	ID_BOOK,	ID_ORG,		ID_CHAP,	ID_SCHOOL,
  NUM_ID
}sort_ids;

char* buffer;
gsize length;
GtkListStore *store;
gboolean buf_mod;

typedef struct {
  GtkWidget *combo, *entry1, *entryAuth, *entryEditor, *entryTitle,
	    *entryPublisher, *entryVol, *entryNumber, *entryPage, *entryMon,
	    *entryNote, *entryKey, *entryYear, *entryAddress, *entryEdition,
	    *entryJournal, *entrySeries, *entryBooktitle, *entryOrg,
	    *entryChap, *entrySchool;
} Entries;

extern GtkTreeStore *treestore;

