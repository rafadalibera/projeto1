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

newspaper: 	T_NEWSPAPER '{' T_TITLE '=' T_STRING  T_DATE '=' T_STRING  structure news_list '}' {	
		FILE *F = fopen("newspaper.htm", "w"); 
		fprintf(F, "<html>\n");
		fprintf(F, "	<head>\n");
		fprintf(F, "		<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"); 
		fprintf(F, "		<title> SEMPRE ONLINE. </title>\n");
		fprintf(F, "		<link rel=\"stylesheet\" type=\"text/css\" href=\"./style/style.css\" media=\"screen\">\n");
		fprintf(F, "		<script type=\"text/javascript\"> </script>\n");
		fprintf(F, "		<style type=\"text/css\"></style>\n");
		fprintf(F, "	</head>\n");
		fprintf(F, "	<body style=\"\">\n");
		fprintf(F, "		<div id=\"header\">\n");
		fprintf(F, "			<div id=\"logo\">\n");
		fprintf(F, "				<h1> %s </h1>\n", $5);
		fprintf(F, "				<p> %s </p>\n", $8);
		fprintf(F, "			</div>\n");
		fprintf(F, "		</div>\n");
		fclose(F);
	} 


structure:
			T_STRUCTURE '{' T_COL '=' T_NUM T_SHOW '=' id_list '}'
		| 	T_STRUCTURE '{' T_COL '=' T_NUM T_SHOW '=' show_list '}'


news_list :
		news
	|	news news_list

news:
	T_ID '{' a_news structure '}'  

a_news:
			T_TITLE '=' T_STRING T_ABSTRACT '=' T_STRING T_AUTHOR '=' T_STRING 
		|	T_TITLE '=' T_STRING T_AUTHOR '=' T_STRING T_ABSTRACT '=' T_STRING 
		|	T_AUTHOR '=' T_STRING T_TITLE '=' T_STRING T_ABSTRACT '=' T_STRING
		|	T_AUTHOR '=' T_STRING T_ABSTRACT '=' T_STRING T_TITLE '=' T_STRING
		|	T_ABSTRACT '=' T_STRING T_AUTHOR '=' T_STRING T_TITLE '=' T_STRING
		|	T_ABSTRACT '=' T_STRING T_TITLE '=' T_STRING T_AUTHOR '=' T_STRING



id_list:
		T_ID 		
	| 	id_list ',' T_ID

show_list:
	/* empty */
	| T_TITLE 
	| T_ABSTRACT
	| T_AUTHOR
	| T_IMAGE
	| T_SOURCE
	| T_DATE
	| T_TEXT
	|	show_list ',' T_TITLE 
	|	show_list ',' T_ABSTRACT 
	|	show_list ',' T_AUTHOR
	|	show_list ',' T_IMAGE
	|	show_list ',' T_SOURCE
	|	show_list ',' T_DATE
	|	show_list ',' T_TEXT
 

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


