%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

%}
 
%union{
	char *str;
	int  intval;
}

%token <str> T_ID
%token T_NEWSPAPER
%token T_TITLE
%token T_ABSTRACT
%token T_TEXT
%token T_SOURCE
%token T_IMAGE
%token T_AUTHOR
%token T_STRUCTURE
%token T_ITEM
%token T_DATE
%token <str> T_STRINGs

%error-verbose


%%

newspaper: 	T_NEWSPAPER '=' '{' T_ID '}' { printf("%s", $4); } 
	 
 
%%
 
int yyerror(const char* errmsg)
{
	printf("\n*** Erro: %s\n", errmsg);
}
 
int yywrap(void) { return 1; }
 
int main(int argc, char** argv)
{
     yyparse();
     return 0;
}


