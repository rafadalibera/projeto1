/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "util.h"
/*
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

ListaNoticias listaNoticias;
*/
//Converte uma string para letras maiusculas. Nao esquecer de dar free na memoria retornada depois de usar se nao for mais necessaria
char *StringToUpper(char * stringOriginal){
	int i = 0;
	char * retorno = (char *)calloc(strlen(stringOriginal) + 1, sizeof(char));

	for (i = 0; i < strlen(stringOriginal); i++){
		retorno[i] = (char)toupper(stringOriginal[i]);
	}
	retorno[i] = '\0';
	return retorno;
}

//Os argumentos precisam ser strings bem formadas (com NUL no final). Todos os objetos sao reinstanciados, ou seja, pode dar free nos fontes depois
//Nao mudar abstrac para abstract. Palavra reservada.
Noticia NewNoticia(char * nomeObjeto, char * title, char * abstrac, char * author, char * date, char * image, char * source, char * text, int numCol){
	Noticia retorno;
	int i = 0;

	retorno.NomeObjeto = (char *)calloc(strlen(nomeObjeto) + 1, sizeof(char));
	retorno.NomeObjeto = StringToUpper(nomeObjeto);

	retorno.Abstract = (char*)calloc(strlen(abstrac) + 1, sizeof(char));
	strcpy(retorno.Abstract, abstrac);

	retorno.Author = (char*)calloc(strlen(author) + 1, sizeof(char));
	strcpy(retorno.Author, author);

	retorno.Date = (char*)calloc(strlen(date) + 1, sizeof(char));
	strcpy(retorno.Date, date);

	retorno.Image = (char*)calloc(strlen(image) + 1, sizeof(char*));
	strcpy(retorno.Image, image);

	retorno.Source = (char *)calloc(strlen(source) + 1, sizeof(char));
	strcpy(retorno.Source, source);

	retorno.Text = (char *)calloc(strlen(text), sizeof(char));
	strcpy(retorno.Text, text);

	retorno.Title = (char *)calloc(strlen(title), sizeof(char));
	strcpy(retorno.Title, title);

	retorno.numCol = numCol;

	for (i = 0; i < 7; i++){
		retorno.listaPropriedades[i] = Invalido;
	}

	retorno.buscada = 0;
	retorno.posicaoNoticia = -1;
	return retorno;
}

//Marca um dos itens da lista de atributos a serem mostrados pela noticia. 
//Notar que o objeto eh eh adicionado numa lista, para manter a ordem em que isso aparece
//no structure da noticia. Aqui eu mesmo jah pego coisas do tipo o usuario tentando 
//declarar para mostrar duas vezes o mesmo objeto.
void MarcarMostrarObjetoNaNoticia(Noticia * noticia, TipoPropriedade tipoObjeto){
	int i = 0;
	
	//Primeiro varre pra testar se o tipoObjeto sendo adicionado jah foi adicionado. Se isso acontecer eh uma condicao de warning.
	for (i = 0; i < 7; i++){
		if ((*noticia).listaPropriedades[i] == tipoObjeto){
			fprintf(stderr, "\nWarning: Noticia %s possui atributos repetidos sendo mostrados\n", (*noticia).NomeObjeto);
		}
	}

	//Aqui coloca o objeto desejado na lista
	for (i = 0; i < 7; i++){
		if ((*noticia).listaPropriedades[i] == Invalido){
			(*noticia).listaPropriedades[i] = tipoObjeto;
			return;
		}
	}
	//Na pagina tem lugar para uma vez cada atributo (ou seja, 7 atributos). Tentou mostrar mais que 7 joga warning.
	fprintf(stderr, "\nWarning: Noticia %s tentando mostrar mais objetos que o possivel.\n", (*noticia).NomeObjeto);
}

void MostrarPropriedade(Noticia * noticia, char * nomePropriedade){
	char * nomePropriedadeMaiuscula = StringToUpper(nomePropriedade);
	if (strcmp(nomePropriedadeMaiuscula, "TITLE") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Title);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "ABSTRACT") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Abstract);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "AUTHOR") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Author);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "DATE") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Date);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "IMAGE") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Image);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "SOURCE") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Source);
	}
	else if (strcmp(nomePropriedadeMaiuscula, "TEXT") == 0){
		MarcarMostrarObjetoNaNoticia(noticia, Text);
	}
	else{
		printf("\nWarning - Token nao reconhecido\n");
	}
	free(nomePropriedadeMaiuscula);
}


//Sinaliza que a noticia apontada deve ser impressa no final.
//nomeToken eh o nome da noticia digitado no codigo que vem do token
//Se nao achar a noticia na lista joga no stderror que o usuario digitou alguma besteira
//Deve ser chamado na ordem do structure da news, pois a ordem de chamada influencia
//na hora da leitura
void MarcarNoticiaParaExibicao(ListaNoticias * listaNoticias, char * nomeToken){
	int ultimaNoticia = -2;
	char *nomeTokenMaiusculo = StringToUpper(nomeToken); //Precisa converter o nome do token para maiusculas, pois o nome do objeto eh sempre em maiusculas (para ser case insensitive)
	int i = 0;

	for (i = 0; i < (*listaNoticias).tamanho; i++){ //pega a posicao atribuida da ultima noticia
		if ((*listaNoticias).valores[i].posicaoNoticia > ultimaNoticia)
			ultimaNoticia = (*listaNoticias).valores[i].posicaoNoticia;
	}
	ultimaNoticia++; //Representa a posicao da proxima noticia

	//Faz a busca na lista
	for (i = 0; i < (*listaNoticias).tamanho; i++){
		if (strcmp((*listaNoticias).valores[i].NomeObjeto, nomeTokenMaiusculo) == 0 && (*listaNoticias).valores[i].posicaoNoticia == -1){
			(*listaNoticias).valores[i].posicaoNoticia = ultimaNoticia;
			free(nomeTokenMaiusculo);
			return;
		}
	}

	fprintf(stderr, "\nWarning: Noticia %s nao faz parte das noticias declaradas\n", nomeTokenMaiusculo);
	free(nomeTokenMaiusculo);
	return;
}

//Cria nova instancia de uma lista de noticias
ListaNoticias NewListaNoticias(int capacidade){
	ListaNoticias retorno;

	retorno.capacidade = capacidade;
	retorno.tamanho = 0;
	retorno.valores = (Noticia *)calloc(capacidade, sizeof(Noticia));
	
	return retorno;
}

//Adiciona uma noticia no final da lista de noticias
void AppendElemento(ListaNoticias * listaNoticias, Noticia novaNoticia){
	if ((*listaNoticias).tamanho + 1 > (*listaNoticias).capacidade){
		(*listaNoticias).capacidade = 2 * (*listaNoticias).capacidade;
		(*listaNoticias).valores = (Noticia *)realloc((*listaNoticias).valores, (*listaNoticias).capacidade * sizeof(int));
	}

	(*listaNoticias).valores[(*listaNoticias).tamanho] = novaNoticia;
	(*listaNoticias).tamanho++;
}

//Quando chamada decide a proxima noticia a ser colocada na tela.
//Mantem a ordem digitada pelo usuario.
//Retorna a proxima noticia
Noticia * BuscaProximaNoticia(ListaNoticias * listaNoticias){
	int i = 0;
	int menorEncontrado = 2147483647;
	Noticia * retorno = NULL;

	for (i = 0; i < (*listaNoticias).tamanho; i++){
		if (!(*listaNoticias).valores[i].buscada && (*listaNoticias).valores[i].posicaoNoticia < menorEncontrado && (*listaNoticias).valores[i].posicaoNoticia >= 0){
			retorno = &(*listaNoticias).valores[i];
			menorEncontrado = (*retorno).posicaoNoticia;
		}
	}

	if (retorno != NULL)
		(*retorno).buscada = 1;

	return retorno;
}

//Testa se todas as noticias jah foram buscada e o loop pode terminar. 0 se ainda tiver alguma coisa para buscar, 1 se jah tiver pesquisado tudo
int TestaSeTodasNoticiasJahForamBuscadas(ListaNoticias * listaNoticias){
	int i = 0;
	int retorno = 1;
	for (i = 0; i < (*listaNoticias).tamanho; i++){
		if ((*listaNoticias).valores[i].posicaoNoticia >= 0) //So faz a varredura dentre as noticias que devem ser exibidas
			retorno = retorno * (*listaNoticias).valores[i].buscada;
	}
	return retorno;
}

//Encontra o maior numero de colunas de todas as noticias. 
//Se isso for maior que o colspan informado no newspaper, tah errado e precisa abortar.
int TestaPorMaiorColSpan(ListaNoticias * listaNoticias, int maxColSpan){
	int i = 0;
	for (i = 0; i < (*listaNoticias).tamanho; i++){
		if ((*listaNoticias).valores[i].numCol > maxColSpan)
			return 1;
	}
	return 0;
}

//Cria o arquivo HTML do corpo do texto de uma noticia
//Nao precisa mandar .html no nome
void ImprimeTextoDeUmaNoticia(Noticia noticia, char * nomeSaidaHtml){
	char * nomeComExtensao = (char *)calloc(strlen(nomeSaidaHtml) + 6, sizeof(char));
	strcpy(nomeComExtensao, nomeSaidaHtml);
	FILE * arquivoSaida = fopen(strcat(nomeComExtensao, ".html"), "w");

	//Imprime a noticia
	fprintf(arquivoSaida, "<html><head><link href=\"styleJanelas.css\" rel=\"stylesheet\" type=\"text/css\"></head><body><center><h2>%s</h2></center><br>%s</body></html>", noticia.Title, noticia.Text);

	fclose(arquivoSaida);
	free(nomeComExtensao);
}

//Dalibera, essa daqui eh a funcao que imprime uma das noticias.
//Faz com fprintf ao inves de retornar string. Eu chamo essa funcao 
//sempre que eu quiser imprimir uma das noticias. Voce fica no escopo <td></td>.
//Detalhe que, mais pra frente, eh aqui que vamos ter de dar um jeito de formatar 
//as coisas do wikipedia. Mas acho que isso eh sussa.
void ImprimeUmaNoticia(Noticia noticia, FILE * F){
	int i = 0;
	fprintf(F, "<td width=\"337\" align=\"justify\" valign=\"top\" colspan=\"%d\">\n", noticia.numCol);
	
	for (i = 0; i < 7; i++){
		if (noticia.listaPropriedades[i] == Title){
			if (strcmp("", noticia.Text)){
				fprintf(F, "<h2><a href onclick=\"window.open('%s.html', '_blank', 'width=720,height=500,toolbar=no,scrollbars=yes,resizable=no');\">%s</a></h2>\n", noticia.NomeObjeto, noticia.Title);
				ImprimeTextoDeUmaNoticia(noticia, noticia.NomeObjeto);
			}
			else
				fprintf(F, "<h2>%s</h2>\n", noticia.Title);
		}
		else if (noticia.listaPropriedades[i] == Date){
			fprintf(F, "<p>\n");
			fprintf(F, "%s\n", noticia.Date);
			fprintf(F, "<p>\n");
			fprintf(F, "<br>\n");
		}
		else if (noticia.listaPropriedades[i] == Abstract){
			fprintf(F, "<p>\n");
			fprintf(F, "%s\n", noticia.Abstract);
			fprintf(F, "<p>\n");
			fprintf(F, "<br>\n");
		}
		else if (noticia.listaPropriedades[i] == Image){
			fprintf(F, "<div id=\"figura\"><p> <img class=\"escala\" src=\"%s\"> </p></div> \n", noticia.Image);
			fprintf(F, "<br>\n");
		}
		else if (noticia.listaPropriedades[i] == Source){
			fprintf(F, "<p>\n");
			fprintf(F, "<b>Fonte: </b> <a href=\"%s\" target=\"_blank\">%s</a> \n", noticia.Source, noticia.Source);
			fprintf(F, "<p>\n");
			fprintf(F, "<br>\n");
		}
		else if (noticia.listaPropriedades[i] == Author){
			fprintf(F, "<p>\n");
			fprintf(F, "<b>Autor: </b> %s \n", noticia.Author);
			fprintf(F, "<p>\n");
			fprintf(F, "<br>\n");
		}
	}

	fprintf(F, "</td>\n");
}


void ImprimeTodasNoticias(ListaNoticias * listaNoticias, int colspan, FILE * arquivo){
	int colsDisponiveis = colspan;
	Noticia * proximaNoticia = NULL;
	
	do{
		colsDisponiveis = colspan;
		fprintf(arquivo, "<tr>");
		while (colsDisponiveis > 0){
			proximaNoticia = BuscaProximaNoticia(listaNoticias);
			if (proximaNoticia != NULL){
				colsDisponiveis -= (*proximaNoticia).numCol;
				ImprimeUmaNoticia((*proximaNoticia), arquivo);
				if (colsDisponiveis < 0)
					fprintf(stderr, "\nWarning: Noticia %s extrapola numero de colunas disponiveis do jornal\n", (*proximaNoticia).NomeObjeto);
			}
			else{
				break;
			}
		}
		fprintf(arquivo, "</tr>");
	} while (!TestaSeTodasNoticiasJahForamBuscadas(listaNoticias));
}

//Imprime o preambulo. Coloquei um javascript pra atualizar o dia automaticamente.
void ImprimePreambulo(FILE * arquivo){
	fprintf(arquivo, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title> SEMPRE ONLINE. </title><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" media=\"screen\"><script type=\"text/javascript\"> </script><style type=\"text/css\"></style></head><body style=\"\"><div id=\"header\"> <div id=\"logo\"> <h1><a><span>SEMPRE ONLINE.</span>O jornal em tempo real. </a></h1><p><script language=\"JavaScript\">var now = new Date();var dayNames = new Array(\"Domingo\",\"Segunda - Feira\",\"Terca - Feira\",\"Quarta - Feira\",\"Quinta - Feira\",\"Sexta - Feira\",\"Sabado\");var monNames = new Array(\"Janeiro\",\"Fevereiro\",\"Marco\",\"Abril\",\"Maio\",\"Junho\",\"Julho\",\"Agosto\",\"Setembro\",\"Outubro\",\"Novembro\",\"Dezembro\");document.write(dayNames[now.getDay()] + \", \" + now.getDate() + \" de \" + monNames[now.getMonth()] + \" de \" +  now.getFullYear());</script></p></div><div id=\"separador\"></div></div><table cellspacing=\"0\" cellpadding=\"8\" width=\"1024\" border=\"0\"><tbody>");
}

//Imprime as closing tags
void ImprimeTermino(FILE * arquivo){
	fprintf(arquivo, "</tbody></table></body></html>");
}

//Imprime a pagina web, com posse das noticias
void ImprimePaginaWeb(char * nomeSaidaHtml, ListaNoticias * listaNoticias, int colspan){ //Chamar essa funcao quando ele reduzir o newspaper, porque jah se tem todas as informacoes necessarias
	FILE * arquivo = fopen(nomeSaidaHtml, "w");
	ImprimePreambulo(arquivo);
	ImprimeTodasNoticias(listaNoticias, colspan, arquivo);
	ImprimeTermino(arquivo);
	fclose(arquivo);
}

int stringLength (char * str) {
	int i = 0;

	while(str[i] != '\0') {
		i++;
	}

	return i;
}

char* concat(char *str1, char *str2, char *str3)
{
	int length = stringLength(str1) + stringLength(str2) + stringLength(str3);
	char *str = (char *) malloc ((length + 1) * sizeof(char));

	strcpy (str, str1);
	str = strcat(str, str2);
	str = strcat(str, str3);

	return str;
}


void MarcarTodasNoticias (ListaNoticias * listaNoticias, char * lista) {
	char temp[10];
	int length;
	int i=0, j=0;

	temp[0] = '\0';

	length = stringLength(lista);

	while (i < length+1) {
		if (lista[i] == ',' || lista[i] == '\0') {
			temp[j] = '\0';
			//printf("-> %s\n", temp);
			MarcarNoticiaParaExibicao(listaNoticias, temp);
			j = 0;
			temp[j] = '\0';
			i++;
		} else {
			if (lista[i] != ' ') {
				temp[j] = lista[i];
				j++;
			}
			i++;
		}
	}
}

void MarcarMostrarTodosObjetosNaNoticia(Noticia * noticia, char * lista) {
	char temp[10];
	int length;
	int i=0, j=0;

	temp[0] = '\0';

	length = stringLength(lista);

	while (i < length+1) {
		if (lista[i] == ',' || lista[i] == '\0') {
			temp[j] = '\0';
			//printf("-> %s\n", temp);
			MostrarPropriedade(noticia, temp);
			j = 0;
			temp[j] = '\0';
			i++;
		} else {
			if (lista[i] != ' ') {
				temp[j] = lista[i];
				j++;
			}
			i++;
		}
	}	
}

//******************************************************
DicionarioNoticia NewDicionarioNoticia(){
	DicionarioNoticia retorno;
	retorno.Title = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Title, "");

	retorno.Abstract = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Abstract, "");

	retorno.Author = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Author, "");

	retorno.Date = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Date, "");

	retorno.Image = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Image, "");

	retorno.Source = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Source, "");

	retorno.Text = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy(retorno.Text, "");

	return retorno;
}

void CleanDicionarioNoticia (DicionarioNoticia * dic) {
	free(dic->Title);
	free(dic->Abstract);
	free(dic->Author);
	free(dic->Date);
	free(dic->Image);
	free(dic->Source);
	free(dic->Text);

	(*dic).Title = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Title, "");

	(*dic).Abstract = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Abstract, "");

	(*dic).Author = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Author, "");

	(*dic).Date = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Date, "");

	(*dic).Image = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Image, "");

	(*dic).Source = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Source, "");

	(*dic).Text = (char *)calloc(strlen("") + 1, sizeof(char));
	strcpy((*dic).Text, "");
}

void AdicionarChave(DicionarioNoticia * dic, char * chave, char * valor){
	char * chaveMaiuscula = StringToUpper(chave);
	if (strcmp(chaveMaiuscula, "TITLE") == 0){
		if (strcmp((*dic).Title, "") == 0){
			free((*dic).Title);
			(*dic).Title = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Title, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo TITLE repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "ABSTRACT") == 0){
		if (strcmp((*dic).Abstract, "") == 0){
			free((*dic).Abstract);
			(*dic).Abstract = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Abstract, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo ABSTRACT repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "AUTHOR") == 0){
		if (strcmp((*dic).Author, "") == 0){
			free((*dic).Author);
			(*dic).Author = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Author, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo AUTHOR repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "DATE") == 0){
		if (strcmp((*dic).Date, "") == 0){
			free((*dic).Date);
			(*dic).Date = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Date, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo DATE repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "IMAGE") == 0){
		if (strcmp((*dic).Image, "") == 0){
			free((*dic).Image);
			(*dic).Image = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Image, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo IMAGE repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "SOURCE") == 0){
		if (strcmp((*dic).Source, "") == 0){
			free((*dic).Source);
			(*dic).Source = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Source, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo SOURCE repetido no jornal\n");
		}
	}
	else if (strcmp(chaveMaiuscula, "TEXT") == 0){
		if (strcmp((*dic).Text, "") == 0){
			free((*dic).Text);
			(*dic).Text = (char *)calloc(strlen(valor) + 1, sizeof(char));
			strcpy((*dic).Text, valor);
		}
		else{
			fprintf(stderr, "\nWarning - Atributo TEXT repetido no jornal\n");
		}
	}
	else{
		fprintf(stderr, "\nWarning - Atributo desconhecido no jornal\n");
	}


	free(chaveMaiuscula);
}

//Recebe a URL e o texto a ser exibido. Retorna uma string html formatada que já faça isso
char * RetornaLinkTexto(char * url, char * textoExibido);

//Recebe a URL de uma imagem e sua legenda e retorna um HTML formatado que mostre a imagem
char * RetornaImagem(char * url, char * legenda);

char * RetornaTextoTitulo(char * texto);

char * RetornaTextoItalico(char * texto);

char * RetornaTextoNegrito(char * texto);

char * RetornaTextoNegritoItalico(char * texto);

char * AdicionarTextoNumerado(int nivel, char * texto);

char * AdicionarBullet(int nivel, char * texto);

//******************************************************


/* Line 371 of yacc.c  */
#line 652 "parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
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
     T_SSA = 270,
     T_COL = 271,
     T_SHOW = 272,
     T_NUM = 273,
     T_ASP = 274
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 585 "parser.y"

	char *str;
	int  intval;
	struct NewsStructure {
		int coluna;
		char * lista;
	} newsStructure;

	Noticia noticia;


/* Line 387 of yacc.c  */
#line 726 "parser.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 754 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNRULES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   274

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    22,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    24,     2,    26,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    25,
       2,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    20,     2,    23,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,    19,    29,    39,    41,    44,    50,    56,
      62,    68,    74,    80,    86,    92,    99,   106,   113,   120,
     127,   134,   141,   143,   147,   149,   151,   153,   155,   157,
     159,   161,   164,   167,   170,   173,   176,   179,   182,   183,
     185,   187,   189,   191,   193,   195,   197,   201,   205,   209,
     213,   217,   221
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      28,     0,    -1,     4,    20,     5,    21,    22,    34,    22,
      13,    21,    22,    34,    22,    29,    30,    23,    -1,    11,
      20,    16,    21,    18,    17,    21,    33,    23,    -1,    11,
      20,    16,    21,    18,    17,    21,    35,    23,    -1,    31,
      -1,    31,    30,    -1,     3,    20,    32,    29,    23,    -1,
       5,    21,    22,    34,    22,    -1,     6,    21,    22,    34,
      22,    -1,    10,    21,    22,    34,    22,    -1,     9,    21,
      22,    34,    22,    -1,     8,    21,    22,    34,    22,    -1,
      13,    21,    22,    34,    22,    -1,     7,    21,    22,    34,
      22,    -1,    32,     5,    21,    22,    34,    22,    -1,    32,
       6,    21,    22,    34,    22,    -1,    32,    10,    21,    22,
      34,    22,    -1,    32,     9,    21,    22,    34,    22,    -1,
      32,     8,    21,    22,    34,    22,    -1,    32,    13,    21,
      22,    34,    22,    -1,    32,     7,    21,    22,    34,    22,
      -1,     3,    -1,    33,    24,     3,    -1,    14,    -1,    17,
      -1,    24,    -1,    25,    -1,    26,    -1,     3,    -1,    18,
      -1,    34,    14,    -1,    34,     3,    -1,    34,    18,    -1,
      34,    24,    -1,    34,    26,    -1,    34,    25,    -1,    34,
      17,    -1,    -1,     5,    -1,     6,    -1,    10,    -1,     9,
      -1,     8,    -1,    13,    -1,     7,    -1,    35,    24,     5,
      -1,    35,    24,     6,    -1,    35,    24,    10,    -1,    35,
      24,     9,    -1,    35,    24,     8,    -1,    35,    24,    13,
      -1,    35,    24,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   625,   625,   635,   644,   657,   662,   670,   685,   686,
     687,   688,   689,   690,   692,   693,   694,   695,   696,   697,
     698,   700,   848,   849,   853,   854,   855,   856,   857,   858,
     859,   864,   865,   866,   871,   872,   873,   874,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   891,   892
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ID", "T_NEWSPAPER", "T_TITLE",
  "T_ABSTRACT", "T_TEXT", "T_SOURCE", "T_IMAGE", "T_AUTHOR", "T_STRUCTURE",
  "T_ITEM", "T_DATE", "T_STRING", "T_SSA", "T_COL", "T_SHOW", "T_NUM",
  "T_ASP", "'{'", "'='", "'\"'", "'}'", "','", "';'", "'.'", "$accept",
  "newspaper", "structure", "news_list", "news", "a_news", "id_list",
  "word_list", "show_list", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     123,    61,    34,   125,    44,    59,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    27,    28,    29,    29,    30,    30,    31,    32,    32,
      32,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    33,    33,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    35,    35,
      35,    35,    35,    35,    35,    35,    35,    35,    35,    35,
      35,    35,    35
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,    15,     9,     9,     1,     2,     5,     5,     5,
       5,     5,     5,     5,     5,     6,     6,     6,     6,     6,
       6,     6,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,    29,    24,
      25,    30,    26,    27,    28,     0,    32,    31,    37,    33,
       0,    34,    36,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     2,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    39,    40,    45,    43,
      42,    41,    44,     0,     0,     8,     9,    14,    12,    11,
      10,    13,     0,     0,     0,     0,     0,     0,     0,     3,
       0,     4,     0,    15,    16,    21,    19,    18,    17,    20,
      23,    46,    47,    52,    50,    49,    48,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    30,    33,    34,    47,   103,    15,   104
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -27
static const yytype_int16 yypact[] =
{
       7,   -18,     4,    14,   -27,     9,    -1,   224,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,    -2,   -27,   -27,   -27,   -27,
      18,   -27,   -27,   -27,    13,    10,   224,    11,    36,    26,
      45,    34,    31,    32,    45,    33,   255,   -27,   -27,    38,
      49,    53,    54,    57,    62,    66,    67,   246,    59,    69,
      74,    78,    79,    80,    82,    87,    92,    93,    96,   101,
     105,   106,   109,   112,   118,   224,   224,   224,   224,   224,
     224,   224,   119,   121,   126,   130,   131,   132,   134,   -27,
       0,    35,    55,    68,    81,    94,   107,   120,   224,   224,
     224,   224,   224,   224,   224,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,    -6,     3,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   133,   146,   159,   172,   185,   198,   211,   -27,
      86,   -27,   264,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -27,   -27,   114,   135,   -27,   -27,   -27,   -26,   -27
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      27,    16,     3,    95,     4,    96,    97,    98,    99,   100,
     101,     1,    17,   102,    16,    18,    19,   119,   120,     5,
      20,     7,    21,    22,    23,    17,   121,   122,    18,    19,
       6,    24,    26,    28,    25,    21,    22,    23,    16,    81,
      82,    83,    84,    85,    86,    87,    31,    29,    32,    17,
      35,    36,    18,    19,    39,    37,    48,   105,    16,    21,
      22,    23,   112,   113,   114,   115,   116,   117,   118,    17,
      49,    16,    18,    19,    50,    51,    64,   106,    52,    21,
      22,    23,    17,    53,    16,    18,    19,    54,    55,   130,
     107,    65,    21,    22,    23,    17,    66,    16,    18,    19,
      67,    68,    69,   108,    70,    21,    22,    23,    17,    71,
      16,    18,    19,    72,    73,     0,   109,    74,    21,    22,
      23,    17,    75,    16,    18,    19,    76,    77,     0,   110,
      78,    21,    22,    23,    17,    79,    16,    18,    19,    80,
       0,    88,   111,    89,    21,    22,    23,    17,    90,    16,
      18,    19,    91,    92,    93,   123,    94,    21,    22,    23,
      17,    63,    16,    18,    19,     0,     0,     0,   124,    38,
      21,    22,    23,    17,     0,    16,    18,    19,     0,     0,
       0,   125,     0,    21,    22,    23,    17,     0,    16,    18,
      19,     0,     0,     0,   126,     0,    21,    22,    23,    17,
       0,    16,    18,    19,     0,     0,     0,   127,     0,    21,
      22,    23,    17,     0,    16,    18,    19,     0,     0,     0,
     128,     0,    21,    22,    23,    17,     0,     8,    18,    19,
       0,     0,     0,   129,     0,    21,    22,    23,     9,     0,
       0,    10,    11,     0,     0,     0,     0,     0,    12,    13,
      14,    56,    57,    58,    59,    60,    61,    29,     0,    62,
      40,    41,    42,    43,    44,    45,     0,     0,    46,   131,
     132,   133,   134,   135,   136,     0,     0,   137
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-27)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      26,     3,    20,     3,     0,     5,     6,     7,     8,     9,
      10,     4,    14,    13,     3,    17,    18,    23,    24,     5,
      22,    22,    24,    25,    26,    14,    23,    24,    17,    18,
      21,    13,    22,    22,    21,    24,    25,    26,     3,    65,
      66,    67,    68,    69,    70,    71,    20,    11,     3,    14,
      16,    20,    17,    18,    21,    23,    18,    22,     3,    24,
      25,    26,    88,    89,    90,    91,    92,    93,    94,    14,
      21,     3,    17,    18,    21,    21,    17,    22,    21,    24,
      25,    26,    14,    21,     3,    17,    18,    21,    21,     3,
      22,    22,    24,    25,    26,    14,    22,     3,    17,    18,
      22,    22,    22,    22,    22,    24,    25,    26,    14,    22,
       3,    17,    18,    21,    21,    -1,    22,    21,    24,    25,
      26,    14,    21,     3,    17,    18,    21,    21,    -1,    22,
      21,    24,    25,    26,    14,    23,     3,    17,    18,    21,
      -1,    22,    22,    22,    24,    25,    26,    14,    22,     3,
      17,    18,    22,    22,    22,    22,    22,    24,    25,    26,
      14,    47,     3,    17,    18,    -1,    -1,    -1,    22,    34,
      24,    25,    26,    14,    -1,     3,    17,    18,    -1,    -1,
      -1,    22,    -1,    24,    25,    26,    14,    -1,     3,    17,
      18,    -1,    -1,    -1,    22,    -1,    24,    25,    26,    14,
      -1,     3,    17,    18,    -1,    -1,    -1,    22,    -1,    24,
      25,    26,    14,    -1,     3,    17,    18,    -1,    -1,    -1,
      22,    -1,    24,    25,    26,    14,    -1,     3,    17,    18,
      -1,    -1,    -1,    22,    -1,    24,    25,    26,    14,    -1,
      -1,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
      26,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
       5,     6,     7,     8,     9,    10,    -1,    -1,    13,     5,
       6,     7,     8,     9,    10,    -1,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    28,    20,     0,     5,    21,    22,     3,    14,
      17,    18,    24,    25,    26,    34,     3,    14,    17,    18,
      22,    24,    25,    26,    13,    21,    22,    34,    22,    11,
      29,    20,     3,    30,    31,    16,    20,    23,    30,    21,
       5,     6,     7,     8,     9,    10,    13,    32,    18,    21,
      21,    21,    21,    21,    21,    21,     5,     6,     7,     8,
       9,    10,    13,    29,    17,    22,    22,    22,    22,    22,
      22,    22,    21,    21,    21,    21,    21,    21,    21,    23,
      21,    34,    34,    34,    34,    34,    34,    34,    22,    22,
      22,    22,    22,    22,    22,     3,     5,     6,     7,     8,
       9,    10,    13,    33,    35,    22,    22,    22,    22,    22,
      22,    22,    34,    34,    34,    34,    34,    34,    34,    23,
      24,    23,    24,    22,    22,    22,    22,    22,    22,    22,
       3,     5,     6,     7,     8,     9,    10,    13
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1787 of yacc.c  */
#line 625 "parser.y"
    {	

		MarcarTodasNoticias (&listaNoticias, (yyvsp[(13) - (15)].newsStructure).lista);

		ImprimePaginaWeb("out.htm", &listaNoticias, (yyvsp[(13) - (15)].newsStructure).coluna);
	}
    break;

  case 3:
/* Line 1787 of yacc.c  */
#line 636 "parser.y"
    { 
				struct NewsStructure temp;
				temp.coluna = (yyvsp[(5) - (9)].intval);
				temp.lista = (char *) malloc ((sizeof((yyvsp[(8) - (9)].str)) + 1) * sizeof(char));
				temp.lista = (yyvsp[(8) - (9)].str);

				(yyval.newsStructure) = temp;
			}
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 645 "parser.y"
    { 
				struct NewsStructure temp;
				temp.coluna = (yyvsp[(5) - (9)].intval);
				temp.lista = (char *) malloc ((sizeof((yyvsp[(8) - (9)].str)) + 1) * sizeof(char));
				temp.lista = (yyvsp[(8) - (9)].str);

				(yyval.newsStructure) = temp;
			}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 658 "parser.y"
    {
			AppendElemento(&listaNoticias, (yyvsp[(1) - (1)].noticia));
			printf("%s\n", (yyvsp[(1) - (1)].noticia).Author);
		}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 663 "parser.y"
    {
			AppendElemento(&listaNoticias, (yyvsp[(1) - (2)].noticia));
			printf("%s\n", (yyvsp[(1) - (2)].noticia).Author);
		}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 671 "parser.y"
    {
			Noticia novaNoticia;
			novaNoticia = NewNoticia((yyvsp[(1) - (5)].str), dicionarioNoticia.Title, dicionarioNoticia.Abstract, dicionarioNoticia.Author, dicionarioNoticia.Date, dicionarioNoticia.Image, dicionarioNoticia.Source, dicionarioNoticia.Text, (yyvsp[(4) - (5)].newsStructure).coluna);

			MarcarMostrarTodosObjetosNaNoticia(&novaNoticia, (yyvsp[(4) - (5)].newsStructure).lista); 

			CleanDicionarioNoticia (&dicionarioNoticia);
			dicionarioNoticia = NewDicionarioNoticia();

			(yyval.noticia) = novaNoticia;
		}
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 685 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "title", (yyvsp[(4) - (5)].str));}
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 686 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "abstract", (yyvsp[(4) - (5)].str));}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 687 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "author", (yyvsp[(4) - (5)].str)); }
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 688 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "image", (yyvsp[(4) - (5)].str)); }
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 689 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "source", (yyvsp[(4) - (5)].str)); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 690 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "date", (yyvsp[(4) - (5)].str)); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 692 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "text", (yyvsp[(4) - (5)].str)); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 693 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "title", (yyvsp[(5) - (6)].str)); }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 694 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "abstract", (yyvsp[(5) - (6)].str)); }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 695 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "author", (yyvsp[(5) - (6)].str)); }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 696 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "image", (yyvsp[(5) - (6)].str)); }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 697 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "source", (yyvsp[(5) - (6)].str)); }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 698 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "date", (yyvsp[(5) - (6)].str)); }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 700 "parser.y"
    { AdicionarChave(&dicionarioNoticia, "text", (yyvsp[(5) - (6)].str)); }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 848 "parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 849 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", (yyvsp[(3) - (3)].str)); }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 853 "parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 854 "parser.y"
    { (yyval.str) = "show"; }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 855 "parser.y"
    { (yyval.str) = ",";}
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 856 "parser.y"
    { (yyval.str) = ";";}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 857 "parser.y"
    { (yyval.str) = ".";}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 858 "parser.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str);}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 859 "parser.y"
    {
	char * buffer = (char *)calloc(11, sizeof(char));
	sprintf(buffer, "%d", (yyvsp[(1) - (1)].intval));
	(yyval.str) = buffer;
	}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 864 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (2)].str), " ", (yyvsp[(2) - (2)].str)); }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 865 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (2)].str), " ", (yyvsp[(2) - (2)].str)); }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 866 "parser.y"
    { 
		char * buffer = (char *)calloc(11, sizeof(char));
		sprintf(buffer, "%d", (yyvsp[(2) - (2)].intval));
		(yyval.str) = buffer;
		(yyval.str) = concat((yyvsp[(1) - (2)].str), " ", buffer); }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 871 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (2)].str), ",", " "); }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 872 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (2)].str), ".", " "); }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 873 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (2)].str), ";", " "); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 874 "parser.y"
    {(yyval.str) = concat((yyvsp[(1) - (2)].str), " ", "show"); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 878 "parser.y"
    { (yyval.str) = " "; }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 879 "parser.y"
    { (yyval.str) = "title"; }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 880 "parser.y"
    { (yyval.str) = "abstract"; }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 881 "parser.y"
    { (yyval.str) = "author"; }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 882 "parser.y"
    { (yyval.str) = "image"; }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 883 "parser.y"
    { (yyval.str) = "source"; }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 884 "parser.y"
    { (yyval.str) = "date"; }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 885 "parser.y"
    { (yyval.str) = "text"; }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 886 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "title"); }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 887 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "abstract"); }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 888 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "author"); }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 889 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "image"); }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 890 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "source"); }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 891 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "date"); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 892 "parser.y"
    { (yyval.str) = concat((yyvsp[(1) - (3)].str), ",", "text"); }
    break;


/* Line 1787 of yacc.c  */
#line 2421 "parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 895 "parser.y"

 
int yyerror(const char* errmsg)
{
	printf("\n*** Erro: %s\n", errmsg);
}
 
int yywrap(void) { return 1; }
 
int main(int argc, char** argv)
{
	listaNoticias = NewListaNoticias(10);
	dicionarioNoticia = NewDicionarioNoticia();
    yyparse();
    return 0;
}