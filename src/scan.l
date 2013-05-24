%{
#include "bib.h"
  int line;
%}

%option noyywrap

%x braceV

%%

<braceV>[^{}]* { yylval.sval = strdup(yytext); return VALUE; }
<braceV>[{}]   { return *yytext; }

[A-Za-z][A-Za-z0-9_":]* 	{ yylval.sval = strdup(yytext); return KEY; }
\".*\"              	{ yylval.sval = strndup(yytext+1, yyleng-2); return VALUE; }
[0-9]+                  	{ yylval.sval = strdup(yytext); return VALUE; }
@[A-Za-z][A-Za-z]+      	{ yylval.sval = strdup(yytext+1); return ENTRYTYPE; }
[ \t\n]                 	; /* ignore whitespace */
[{}=,]                  	{ return *yytext; }
#.                       	{ fprintf(stderr, "Unrecognized character %c in input\n", *yytext); }
\n				{line++;printf("%d",line);}

%%

void lex_brace() {
    BEGIN(braceV);
}
void lex_normal() {
    BEGIN(0);
}


