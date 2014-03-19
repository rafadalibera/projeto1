%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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

%}
 
%union{
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

%type <str> id_list show_list T_TITLE T_ABSTRACT T_AUTHOR  T_IMAGE	T_SOURCE T_DATE T_TEXT	
%type <newsStructure> structure
%type <tempNews> a_news
%type <noticia> news


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

		printf("-> %d\n", $9.coluna);

		MarcarTodasNoticias (&listaNoticias, $9.lista);

		ImprimePaginaWeb("out.htm", &listaNoticias, $9.coluna);
	} 


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


news_list :
		news 
		{
			AppendElemento(&listaNoticias, $1);
			printf("%s\n", $1.Author);
		}
	|	news news_list
		{
			AppendElemento(&listaNoticias, $1);
			printf("%s\n", $1.Author);
		}

news:
		T_ID '{' a_news structure '}'
		{
			//struct TempNews a_news_struct = (struct TempNews)$3;
			//struct NewsStructure structure_struct = (struct NewsStructure)$4;

			Noticia novaNoticia;

			novaNoticia = NewNoticia($1, $3.title, $3.abstract, $3.author, $3.date, $3.image, $3.source, $3.text, $4.coluna);
			//printf("%s\n", $4.lista);

			MarcarMostrarTodosObjetosNaNoticia(&novaNoticia, $4.lista); 
			
			$$ = novaNoticia;
		}  

a_news:
			T_TITLE '=' T_STRING T_ABSTRACT '=' T_STRING T_AUTHOR '=' T_STRING 
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.title = $3;
				temp.abstract = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.abstract = $6;
				temp.author = (char *) malloc ((sizeof($9) + 1) * sizeof(char));
				temp.author = $9;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			}	
		|	T_TITLE '=' T_STRING T_AUTHOR '=' T_STRING T_ABSTRACT '=' T_STRING
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.title = $3;
				temp.abstract = (char *) malloc ((sizeof($9) + 1) * sizeof(char));
				temp.abstract = $9;
				temp.author = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.author = $6;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			} 
		|	T_AUTHOR '=' T_STRING T_TITLE '=' T_STRING T_ABSTRACT '=' T_STRING
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.title = $6;
				temp.abstract = (char *) malloc ((sizeof($9) + 1) * sizeof(char));
				temp.abstract = $9;
				temp.author = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.author = $3;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			}
		|	T_AUTHOR '=' T_STRING T_ABSTRACT '=' T_STRING T_TITLE '=' T_STRING
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($9) + 1) * sizeof(char));
				temp.title = $9;
				temp.abstract = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.abstract = $6;
				temp.author = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.author = $3;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			}
		|	T_ABSTRACT '=' T_STRING T_AUTHOR '=' T_STRING T_TITLE '=' T_STRING
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($9) + 1) * sizeof(char));
				temp.title = $9;
				temp.abstract = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.abstract = $3;
				temp.author = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.author = $6;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			}
		|	T_ABSTRACT '=' T_STRING T_TITLE '=' T_STRING T_AUTHOR '=' T_STRING
			{ 
				struct TempNews temp;
				temp.title = (char *) malloc ((sizeof($6) + 1) * sizeof(char));
				temp.title = $6;
				temp.abstract = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.abstract = $3;
				temp.author = (char *) malloc ((sizeof($3) + 1) * sizeof(char));
				temp.author = $3;
				temp.date = (char *) malloc (sizeof(char));
				temp.date = "";
				temp.image = (char *) malloc (sizeof(char));
				temp.image = "";
				temp.source = (char *) malloc (sizeof(char));
				temp.source = "";
				temp.text = (char *) malloc (sizeof(char));
				temp.text = "";

				$$ = temp;
			}



id_list:
		T_ID {$$ = $1;}
	| 	id_list ',' T_ID {$$ = concat($1, ",", $3);}

show_list:
	/* empty */		{$$ = ""}
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
    yyparse();
    return 0;
}