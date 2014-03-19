%{
#include "parser.h"

%}

ID [\*a-z][a-z0-9_]*
NUM [0-9]+
STRING \"[A-Za-z0-9 \?!@#\$%^&*(),\.;/]+\"
WHITESPACE [ \n\r\t]+
ANY_CHAR .

%option case-insensitive

%%

{WHITESPACE} /* ignore */ ;
"COL"			{return T_COL;}
"SHOW"			{return T_SHOW;}
"NEWSPAPER" 	{return T_NEWSPAPER;}
"TITLE" 		{return T_TITLE; }
"DATE"			{return T_DATE;}
"ABSTRACT"		{return T_ABSTRACT;}
"TEXT"			{return T_TEXT;}
"SOURCE"		{return T_SOURCE;}
"IMAGE"			{return T_IMAGE;}
"AUTHOR"		{return T_AUTHOR;}
"STRUCTURE"		{return T_STRUCTURE;}
"ITEM"			{return T_ITEM;}
"{"				{return '{';}
"}"				{return '}';} 
"," 			{return ',';}
";"				{return ';';}
"="				{return '=';}

{ID} {	
	yylval.str = (char *) strdup(yytext);
	return T_ID;
}

{STRING} {
	yylval.str = (char *) strdup(yytext);
	return T_STRING;
}

{NUM} {
	yylval.intval = atoi(strdup(yytext));
	return T_NUM;
}

{ANY_CHAR} {
	printf ("Caracter %c (ASCII=%d) inesperado\n", yytext[0], yytext[0]);
}

%%