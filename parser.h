/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_ID = 258,
     T_NEWSPAPER = 259,
     T_TITLE = 260,
     T_ABSTRACT = 261,
     T_TEXT = 262,
     T_SOURCE = 263,
     T_IMAGE = 264,
     T_AUTHOR = 265,
     T_STRUCTURE = 266,
     T_ITEM = 267,
     T_DATE = 268,
     T_STRING = 269,
     T_COL = 270,
     T_SHOW = 271,
     T_NUM = 272
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
   typedef enum  {
	   Invalido, Title, Abstract, Author, Date, Image, Source, Text
   }TipoPropriedade;

   typedef struct{
	   int buscada; //Esse campo serve soh para dizer se uma noticia jah foi eleita para ser colocada na tela. Necessario na hora de ordenar / decidir.
	   int posicaoNoticia; //-1 significa que essa noticia nao deve ser mostrada. 0 em diante eh a posicao dessa noticia no jornal
	   char * NomeObjeto; //Aqui precisa ir tipo o T_ID "headline1" do codigo, para mais tarde decidirmos quais noticias mostrar, vindo do structure da noticia
	   char * Title;
	   char * Abstract;
	   char * Author;
	   char * Date;
	   char * Image;
	   char * Source;
	   char * Text;
	   int numCol;
	   TipoPropriedade listaPropriedades[7];
   } Noticia;

   typedef struct {
	   int capacidade;
	   int tamanho;
	   Noticia * valores;
   }ListaNoticias;
   typedef union YYSTYPE
{
/* Line 2053 of yacc.c  */
#line 343 "parser.y"

	char *str;
	int  intval;
	struct NewsStructure {
		int coluna;
		char * lista;
	} newsStructure;
	struct TempNews {
		char * title;
		char * abstract;
		char * author;
		char * date;
		char * image;
		char * source;
		char * text;
	} tempNews;

	Noticia noticia;


/* Line 2053 of yacc.c  */
#line 95 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_H_INCLUDED  */