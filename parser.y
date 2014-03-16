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

//Dalibera, essa daqui eh a funcao que imprime uma das noticias.
//Faz com fprintf ao inves de retornar string. Eu chamo essa funcao 
//sempre que eu quiser imprimir uma das noticias. Voce fica no escopo <td></td>.
//Detalhe que, mais pra frente, eh aqui que vamos ter de dar um jeito de formatar 
//as coisas do wikipedia. Mas acho que isso eh sussa.
void ImprimeUmaNoticia(Noticia noticia, FILE * F){

	fprintf(F, "<td width=\"337\" align=\"justify\" valign=\"top\">\n");
	//Print title
	if (noticia.listaPropriedades[0] == 1) {
		fprintf(F, "<h4>%s</h4>\n", noticia.Title);
	}
	//Print Date
	if (noticia.listaPropriedades[3] == 1) {
		fprintf(F, "<p>\n");
		fprintf(F, "%s\n", noticia.Date);
		fprintf(F, "<p>\n");
		fprintf(F, "<br>\n");
	}
	//Print Abstract
	if (noticia.listaPropriedades[1] == 1) {
		fprintf(F, "<p>\n");
		fprintf(F, "%s\n", noticia.Abstract);
		fprintf(F, "<p>\n");
		fprintf(F, "<br>\n");
	}
	//Print Image
	if (noticia.listaPropriedades[4] == 1) {
		fprintf(F, "<center> <img src=\"%s\"> </center> \n", noticia.Image);
		fprintf(F, "<br>\n");
	}
	//Print Text
	if (noticia.listaPropriedades[6] == 1) {
		fprintf(F, "<p>\n");
		fprintf(F, "%s\n", noticia.Text);
		fprintf(F, "<p>\n");
		fprintf(F, "<br>\n");
	}
	//Print Source
	if (noticia.listaPropriedades[5] == 1) {
		fprintf(F, "<p>\n");
		fprintf(F, "<b>Fonte: </b> <a href=\"%s\" target=\"_blank\">%s</a> \n", noticia.Source, noticia.Source);
		fprintf(F, "<p>\n");
		fprintf(F, "<br>\n");
	}
	//Print Author
	if (noticia.listaPropriedades[2] == 1) {
		fprintf(F, "<p>\n");
		fprintf(F, "<b>Autor: </b> %s \n", noticia.Author);
		fprintf(F, "<p>\n");
		fprintf(F, "<br>\n");
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


void TesteMetodos(){
	ListaNoticias lista = NewListaNoticias(10); //O capacity pode por qualquer coisa. Acho que 10 tah bom para nao ficar dando realloc nem gastar infinito memoria. Mas se passar disso ele realoca.

	Noticia not10 = NewNoticia("headline10", "titulo10", "abstract10", "author10", "data10", "imagem10", "fonte10", "texto balbalablab10", 10); //Instancias de teste
	Noticia not1 = NewNoticia("headline1", "titulo1", "abstract1", "author1", "data1", "imagem1", "fonte1", "texto balbalablab10", 1);

	MarcarMostrarObjetoNaNoticia(&not10, Title); //Marca que a noticia not10 deve mostrar titulo e source
	MarcarMostrarObjetoNaNoticia(&not10, Source);

	MarcarMostrarObjetoNaNoticia(&not1, Abstract); //Marca que noticia not1 deve mostrar abstract e author
	MarcarMostrarObjetoNaNoticia(&not1, Author);

	AppendElemento(&lista, not10); //Coloca not10 e not1 na lista
	AppendElemento(&lista, not1);

	MarcarNoticiaParaExibicao(&lista, "headline10"); //Marca que essa noticia deve ir para a tela como a primeira noticia
	MarcarNoticiaParaExibicao(&lista, "headline1"); //Marca essa noticia deve ir para a tela como a proxima (segunda) noticia

	int jornalOverflow = TestaPorMaiorColSpan(&lista, 7); //Deve retornar 1, pois tem uma noticia com 10 lah
	int jornalOverflow2 = TestaPorMaiorColSpan(&lista, 10); //Deve retornar falso, maior noticia tem 10 colunas 

	int aindaNaoPesquisouTudo = !TestaSeTodasNoticiasJahForamBuscadas(&lista); //Tem de dar 1 (negado de 0), pois nenhuma noticia ainda foi eleita para ser impressa

	Noticia * not10ret = BuscaProximaNoticia(&lista); //Escolhe a primeira noticia disponivel
	Noticia * not1ret = BuscaProximaNoticia(&lista); //Escolha a segunda noticia disponivel 

	Noticia * not1ret2 = BuscaProximaNoticia(&lista); // Deve voltar NULL, pois as duas noticias jah foram pesquisadas e nao tem mais nada sem pesquisar

	int jahPesquisouTudo = TestaSeTodasNoticiasJahForamBuscadas(&lista); //Deve retornar 1, pois as duas noticias jah foram eleitas
}

void TesteGeraHtml(){
	ListaNoticias listaNoticias = NewListaNoticias(5);

	Noticia not1 = NewNoticia("head1", "Teste 1", "Essa eh a primeira noticia de testes", "Eu mermo", "Hoje", "sem imagem", "sem fonte", "aqui vai um texto qqr blablabla", 2);
	
	MarcarMostrarObjetoNaNoticia(&not1, Title);
	MarcarMostrarObjetoNaNoticia(&not1, Image);
	MarcarMostrarObjetoNaNoticia(&not1, Abstract);
	AppendElemento(&listaNoticias, not1);

	Noticia not2 = NewNoticia("head2", "Teste 2", "Essa eh a segunda noticia de testes", "Eu mermo denovo", "Hoje", "sem imagem", "sem fonte", "aqui vai mais outroo texto qqr dasdlsjadlaskjd", 1);

	MarcarMostrarObjetoNaNoticia(&not2, Abstract);
	MarcarMostrarObjetoNaNoticia(&not2, Image);
	MarcarMostrarObjetoNaNoticia(&not2, Title);
	AppendElemento(&listaNoticias, not2);
	
	MarcarNoticiaParaExibicao(&listaNoticias, "HEAD1");
	MarcarNoticiaParaExibicao(&listaNoticias, "HEAD2");

	ImprimePaginaWeb("saidaTeste.html", &listaNoticias, 3);
}

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