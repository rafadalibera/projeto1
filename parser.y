%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "util.h"

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
char * RetornaLinkTexto(char * url, char * textoExibido) {
	char * temp;
	temp = concat("<a href=\"", url, "\" target=\"_blank\">");
	return concat("", textoExibido, "</a>");
}

//Recebe a URL de uma imagem e sua legenda e retorna um HTML formatado que mostre a imagem
char * RetornaImagem(char * url, char * legenda) {
	char *temp;
	temp = concat("<div id=\"figura\"><p><img class=\"escala\" src=\"", url, ">");
	return concat ("<p>", legenda, "</div>");
}

char * RetornaTextoTitulo(char * texto) {
	return concat("<h2>", texto, "</h2>");
}

char * RetornaTextoItalico(char * texto) {
	return concat("<i>", texto, "</i>");
}

char * RetornaTextoNegrito(char * texto){
	return concat("<b>", texto, "</b>");
}

char * RetornaTextoNegritoItalico(char * texto) {
	return concat("<b><i>", texto, "</i></b>");
}

char * AdicionarTextoNumerado(int nivel, char * texto) {
	return texto;
}


char * AdicionarTextoEspaco(int nivel, char * texto) {
	return texto;
}

char * RetornaLink(char * texto) {
	char * retorno;
	retorno = concat("<a href=\"", texto, "\"");
	retorno = concat(retorno, ">", texto);
	retorno = concat(retorno, "</a>", "");

	return retorno;
}

char * AdicionarBullet(int nivel, char * texto) {
	int i = 0;
	int j = 0;
	char * retorno = (char *)calloc(strlen(texto) + 10 + 9*4*nivel, sizeof(char));
	retorno = concat("<br>", "", "");
	for (j = 0; j < nivel; j++){
		for (i = 0; i < 4; i++){
			retorno = concat(retorno, "&nbsp; ", "");
		}
	}
	
	retorno = concat(retorno, "&#8226; ", texto);
	retorno = concat(retorno, "<br>", "");
	return retorno;
}

//******************************************************

%}
 
%union{
	char *str;
	int  intval;
	struct NewsStructure {
		int coluna;
		char * lista;
	} newsStructure;

	Noticia noticia;
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
%token <str> T_SSA
%token T_COL
%token T_SHOW
%token <intval> T_NUM
%token T_ASP


%type <str> id_list show_list word_list content_text
%type <newsStructure> structure
%type <noticia> news

%start newspaper

%error-verbose

%%

newspaper: 	T_NEWSPAPER '{' T_TITLE '=' '\"' word_list  '\"' T_DATE '=' '\"' word_list '\"' structure news_list '}' {	

		MarcarTodasNoticias (&listaNoticias, $13.lista);

		ImprimePaginaWeb("out.htm", &listaNoticias, $13.coluna);
	}
;


structure:
			T_STRUCTURE '{' T_COL '=' T_NUM T_SHOW '=' id_list '}' 		
			{ 
				struct NewsStructure temp;
				temp.coluna = $5;
				temp.lista = (char *) malloc ((sizeof($8) + 1) * sizeof(char));
				temp.lista = $8;

				$$ = temp;
			}
		| 	T_STRUCTURE '{' T_COL '=' T_NUM T_SHOW '=' show_list '}'
			{ 
				struct NewsStructure temp;
				temp.coluna = $5;
				temp.lista = (char *) malloc ((sizeof($8) + 1) * sizeof(char));
				temp.lista = $8;

				$$ = temp;
			}
;


news_list :
		news 
		{
			AppendElemento(&listaNoticias, $1);
		}
	|	news news_list
		{
			AppendElemento(&listaNoticias, $1);
		}
;

news:
		T_ID '{' a_news structure '}'
		{
			Noticia novaNoticia;
			novaNoticia = NewNoticia($1, dicionarioNoticia.Title, dicionarioNoticia.Abstract, dicionarioNoticia.Author, dicionarioNoticia.Date, dicionarioNoticia.Image, dicionarioNoticia.Source, dicionarioNoticia.Text, $4.coluna);

			MarcarMostrarTodosObjetosNaNoticia(&novaNoticia, $4.lista); 

			CleanDicionarioNoticia (&dicionarioNoticia);
			dicionarioNoticia = NewDicionarioNoticia();

			$$ = novaNoticia;
		} 
; 

a_news:
	 	T_TITLE '=' '\"' word_list '\"' 	{ AdicionarChave(&dicionarioNoticia, "title", $4);}	 
	| 	T_ABSTRACT '=' '\"' word_list '\"' 	{ AdicionarChave(&dicionarioNoticia, "abstract", $4);}	
	| 	T_AUTHOR '=' '\"' word_list '\"' 	{ AdicionarChave(&dicionarioNoticia, "author", $4); }	
	| 	T_IMAGE '=' '\"' word_list '\"' 	{ AdicionarChave(&dicionarioNoticia, "image", $4); }	
	| 	T_SOURCE '=' '\"' word_list '\"' 	{ AdicionarChave(&dicionarioNoticia, "source", $4); }	
	| 	T_DATE '=' '\"' word_list '\"' 		{ AdicionarChave(&dicionarioNoticia, "date", $4); }	
	| 	T_TEXT '=' '\"' content_text '\"' 		{ AdicionarChave(&dicionarioNoticia, "text", $4); }	
	|	a_news T_TITLE '=' '\"' word_list '\"'		{ AdicionarChave(&dicionarioNoticia, "title", $5); }		 		
	|	a_news T_ABSTRACT '=' '\"' word_list '\"'	{ AdicionarChave(&dicionarioNoticia, "abstract", $5); }	
	|	a_news T_AUTHOR	'=' '\"' word_list '\"'		{ AdicionarChave(&dicionarioNoticia, "author", $5); }	
	|	a_news T_IMAGE '=' '\"' word_list '\"'		{ AdicionarChave(&dicionarioNoticia, "image", $5); }	
	|	a_news T_SOURCE '=' '\"' word_list '\"'		{ AdicionarChave(&dicionarioNoticia, "source", $5); }	
	|	a_news T_DATE '=' '\"' word_list '\"'		{ AdicionarChave(&dicionarioNoticia, "date", $5); }	
	|	a_news T_TEXT '=' '\"' content_text '\"'	{ AdicionarChave(&dicionarioNoticia, "text", $5); }	
;


content_text: 
				/* blank */																	{ $$ = "";}
			| 	content_text word_list 														{ $$ = concat($1, " ", $2);}
			|	content_text '[' word_list '|' word_list ']'								{ $$ = concat($1, " ", RetornaLinkTexto($3, $5));}
			|	content_text '[' word_list ']'												{ $$ = concat($1, " ", RetornaLink($3));}
			|	content_text '[' '[' word_list '|' word_list ']' ']'						{ $$ = concat($1, " ", RetornaImagem($4, $6));}
			|	content_text '=' '=' '=' word_list '=' '=' '='								{ $$ = concat($1, " ", RetornaTextoTitulo($5));}
			|	content_text '\'' '\'' word_list '\'' '\''									{ $$ = concat($1, " ", RetornaTextoItalico($4));}
			|	content_text '\'' '\'' '\'' word_list '\'' '\'' '\''						{ $$ = concat($1, " ", RetornaTextoNegrito($5));}
			|	content_text '\'' '\'' '\'' '\'' '\'' word_list '\'' '\'' '\'' '\'' '\''	{ $$ = concat($1, " ", RetornaTextoNegritoItalico($7));}
			|	content_text '*' word_list 													{ $$ = concat($1, " ", AdicionarBullet(1, $3));}
			|	content_text '*' '*' word_list 												{ $$ = concat($1, " ", AdicionarBullet(2, $4));}
			|	content_text '*' '*' '*' word_list 											{ $$ = concat($1, " ", AdicionarBullet(3, $5));}
			|  	content_text '#' word_list 													{ $$ = concat($1, " ", AdicionarTextoNumerado(1, $3));}
			| 	content_text '#' '#' word_list 												{ $$ = concat($1, " ", AdicionarTextoNumerado(2, $4));}
			|	content_text '#' '#' '#' word_list											{ $$ = concat($1, " ", AdicionarTextoNumerado(3, $5));}
			/*
			|  	content_text ':' word_list 													{ $$ = concat($1, " ", AdicionarTextoEspaco(1, $3));}
			| 	content_text ':' ':' word_list 												{ $$ = concat($1, " ", AdicionarTextoEspaco(2, $4));}
			|	content_text ':' ':' ':' word_list											{ $$ = concat($1, " ", AdicionarTextoEspaco(3, $5));}
			*/
;

id_list:
		T_ID { $$ = $1; }
	| 	id_list ',' T_ID { $$ = concat($1, ",", $3); }
;

word_list:
		T_STRING { $$ = $1; }
	|	T_SHOW { $$ = "show"; }
	|	',' 		{ $$ = ",";}
	|	';' 		{ $$ = ";";}
	|	'.' 		{ $$ = ".";}
	| 	'\?' 		{ $$ = "\?";}
	| 	'!' 		{ $$ = "!";}
	|	'(' 		{ $$ = "(";}
	|	')' 		{ $$ = ")";}
	|	'\\' '\"' 	{ $$ = "\"";}
	|	'\'' 		{ $$ = "\'";}
	| 	':' '/' '/' { $$ = "://";}
	| 	'.' '/' 	{ $$ = "./";}
	| 	':'			{ $$ = ":"; }
	| 	'-'			{ $$ = "-"; }
	| 	'/'			{ $$ = "/"; }
	|	T_ID 		{$$ = $1;}
	|	T_NUM 
		{
			char * buffer = (char *)calloc(11, sizeof(char));
			sprintf(buffer, "%d", $1);
			$$ = buffer;
		}
	| 	word_list T_STRING { $$ = concat($1, " ", $2); }
	| 	word_list T_ID { $$ = concat($1, " ", $2); }
	|	word_list T_NUM 
		{ 
			char * buffer = (char *)calloc(11, sizeof(char));
			sprintf(buffer, "%d", $2);
			$$ = buffer;
			$$ = concat($1, " ", buffer); 
		}
	|	word_list ',' 			{ $$ = concat($1, ",", " "); }
	|	word_list '.' 			{ $$ = concat($1, ".", ""); }
	|	word_list ';' 			{ $$ = concat($1, ";", " "); }
	| 	word_list '\?' 			{ $$ = concat($1, "\?", " "); }
	| 	word_list '!' 			{ $$ = concat($1, "!", " "); }
	|	word_list '(' 			{ $$ = concat($1, "(", " "); }
	|	word_list ')' 			{ $$ = concat($1, ")", " "); }
	|	word_list '\\' '\"' 	{ $$ = concat($1, "\"", " ");}
	|	word_list '\'' 			{ $$ = concat($1, "\'", "");}
	| 	word_list ':' '/' '/'	{ $$ = concat($1, "://", "");}
	| 	word_list '.' '/'		{ $$ = concat($1, "./", "");}
	| 	word_list ':'			{ $$ = concat($1, ":", " ");}
	| 	word_list '-'			{ $$ = concat($1, "-", "");}
	| 	word_list '/'			{ $$ = concat($1, "/", "");}
	|	word_list T_SHOW {$$ = concat($1, " ", "show"); }
;

show_list:
	/* empty */		{ $$ = " "; }
	| 	T_TITLE 	{ $$ = "title"; }
	| 	T_ABSTRACT 	{ $$ = "abstract"; }
	| 	T_AUTHOR 	{ $$ = "author"; }
	| 	T_IMAGE		{ $$ = "image"; }
	| 	T_SOURCE	{ $$ = "source"; }
	| 	T_DATE		{ $$ = "date"; }
	| 	T_TEXT		{ $$ = "text"; }
	|	show_list ',' T_TITLE		{ $$ = concat($1, ",", "title"); } 		
	|	show_list ',' T_ABSTRACT 	{ $$ = concat($1, ",", "abstract"); }
	|	show_list ',' T_AUTHOR		{ $$ = concat($1, ",", "author"); }
	|	show_list ',' T_IMAGE		{ $$ = concat($1, ",", "image"); }
	|	show_list ',' T_SOURCE		{ $$ = concat($1, ",", "source"); }
	|	show_list ',' T_DATE		{ $$ = concat($1, ",", "date"); }
	|	show_list ',' T_TEXT		{ $$ = concat($1, ",", "text"); }
;
 
%%
 
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