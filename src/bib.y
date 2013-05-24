%{
#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include "main.h"
char* concat(char* str1, char* str2);

void lex_brace();
void lex_normal();
#define slen 1064
 int yylex(void);
/*enum
{
  COL_BIB_KEY=0,
  COL_BIB_TYPE,	COL_BIB_AUTHOR,	COL_BIB_YEAR,
  NUM_COLS} ;
*/  
typedef struct {
  char *type;
  char *id;
  GHashTable *table;
} BibEntry;

BibEntry b_entry;
GtkTreeIter siter;
GtkListStore *store;

void yyerror(char *s)
{
  printf("YYERROR : %s\n", s);
}
void parse_entry(BibEntry *bib_entry);

%}

// Symbols.
%union
{
    char    *sval;
};

%token <sval> ENTRYTYPE
%token <sval> VALUE
%token <sval> KEY
%token OBRACE
%token EBRACE
%token QUOTE
%token SEMICOLON 
%type <sval> Value
%type <sval> BraceV
%type <sval> BraceVs
%start Input

%%

Input: Entry
      | Input Entry ;  /* input is zero or more entires */

Entry: 
     ENTRYTYPE '{' KEY ','{ 
         b_entry.type = $1; 
         b_entry.id = $3;
         b_entry.table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);} 
     KeyVals '}' {
         parse_entry(&b_entry);
         g_hash_table_destroy(b_entry.table);
         free(b_entry.type); free(b_entry.id);
         b_entry.table = NULL;
         b_entry.type = b_entry.id = NULL;}
     ;

KeyVals : /* empty */
        | KeyVal
        | KeyVal ',' KeyVals
        ;

KeyVal : KEY '=' Value          { g_hash_table_replace(b_entry.table, $1, $3); }
       ;

Value : '{' { lex_brace(); } BraceVs { lex_normal(); } '}' { $$ = $3; }
      | VALUE
      ;

BraceVs : /* empty */ { $$ = ""; }
        | BraceV BraceVs { $$ = concat($1, $2); }
        ;

BraceV : VALUE
       | '{' BraceVs '}' { $$ = concat(concat("{", $2), "}"); }
       ;
%%


void parse_entry(BibEntry *bib_entry)
{
  char *author = "", *year = "";
  GHashTableIter iter;
  gpointer key, val;
  char **kiter;
  int i;
 char *keys[] = {"id", "type", "author", "year", "title", "publisher", "editor", 
    "volume", "number", "pages", "month", "note", "address", "edition", "journal",
    "series", "book", "chapter", "organization", NULL};
  char *vals[] = {NULL,  NULL,  NULL, NULL, NULL,
    NULL,  NULL,  NULL, NULL, NULL,
    NULL,  NULL,  NULL, NULL, NULL,
    NULL,	 NULL,  NULL, NULL, NULL};

  g_hash_table_iter_init(&iter, bib_entry->table);
  while (g_hash_table_iter_next(&iter, &key, &val)) {
  for (kiter = keys, i = 0; *kiter; kiter++, i++)
    {
    if (!g_ascii_strcasecmp(*kiter, key)) {
    vals[i] = g_strndup(val,slen);
	break;
    }
  }
  }

  gtk_list_store_append (store, &siter);
  gtk_list_store_set (store, &siter,
      COL_BIB_AUTHOR, 		vals[COL_BIB_AUTHOR],
      COL_BIB_TYPE, 		bib_entry->type,
      COL_BIB_KEY, 		bib_entry->id,
      COL_BIB_YEAR, 		vals[COL_BIB_YEAR],
      COL_BIB_TITLE, 		vals[COL_BIB_TITLE],     
      COL_BIB_PUBLISHER, 	vals[COL_BIB_PUBLISHER],
      COL_BIB_EDITOR,  		vals[COL_BIB_EDITOR],
      COL_BIB_VOL, 		vals[COL_BIB_VOL],
      COL_BIB_NUMBER, 		vals[COL_BIB_NUMBER],
      COL_BIB_PAGE, 		vals[COL_BIB_PAGE],
      COL_BIB_MON, 		vals[COL_BIB_MON],
      COL_BIB_NOTE, 		vals[COL_BIB_NOTE],
      COL_BIB_ADDRESS, 		vals[COL_BIB_ADDRESS],
      COL_BIB_EDITION, 		vals[COL_BIB_EDITION],
      COL_BIB_JOURNAL, 		vals[COL_BIB_JOURNAL],
      COL_BIB_SERIES, 		vals[COL_BIB_SERIES],
      COL_BIB_BOOK, 		vals[COL_BIB_BOOK],
      COL_BIB_ORG, 		vals[COL_BIB_ORG],
      COL_BIB_CHAP, 		vals[COL_BIB_CHAP],
      COL_BIB_SCHOOL, 		vals[COL_BIB_SCHOOL],
      -1);
}

char* concat(char* str1, char* str2) {
    char* ret = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(ret, str1);
    strcat(ret, str2);
    return ret;
}
