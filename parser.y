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
%token <str> T_STRING
%token T_COL
%token T_SHOW
%token <intval> T_NUM

%error-verbose


%%

newspaper: 	T_NEWSPAPER '{' T_TITLE '=' T_STRING  T_DATE '=' T_STRING  structure '}' {  } 


news:
	T_STRING '{' a_news structure '}' 

structure:
	T_STRUCTURE '{' T_COL '=' T_NUM T_SHOW '=' string_list '}'


news_list :
		news
	|	news news_list 

a_news:
			T_TITLE '=' T_STRING '\n' T_ABSTRACT '=' T_STRING '\n' T_AUTHOR '=' T_STRING '\n' 
		|	T_TITLE '=' T_STRING '\n' T_AUTHOR '=' T_STRING '\n' T_ABSTRACT '=' T_STRING '\n'
		|	T_AUTHOR '=' T_STRING '\n' T_TITLE '=' T_STRING '\n' T_ABSTRACT '=' T_STRING '\n'
		|	T_AUTHOR '=' T_STRING '\n' T_ABSTRACT '=' T_STRING '\n' T_TITLE '=' T_STRING '\n'
		|	T_ABSTRACT '=' T_STRING '\n' T_AUTHOR '=' T_STRING '\n' T_TITLE '=' T_STRING '\n'
		|	T_ABSTRACT '=' T_STRING '\n' T_TITLE '=' T_STRING '\n' T_AUTHOR '=' T_STRING '\n'



string_list:
		T_STRING 		
	| 	string_list ',' T_STRING 	
;
 
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


