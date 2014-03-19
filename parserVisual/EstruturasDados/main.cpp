#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef enum  {
	Invalido, Title, Abstract, Author, Date, Image, Source, Text
} TipoPropriedade;

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
} ListaNoticias;

typedef struct{
	char * Title;
	char * Abstract;
	char * Author;
	char * Date;
	char * Image;
	char * Source;
	char * Text;
} DicionarioNoticia;

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
}

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
		fprintf(stderr, "\nWarning - Token nao reconhecido\n");
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

char * TrimAspasString(char * string){
	char * retorno = (char *)calloc(strlen(string), sizeof(char));
	strcpy(retorno, string + 1);
	retorno[strlen(retorno) - 1] = '\0';
	return retorno;
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
			else{
				fprintf(F, "<h2>%s</h2>\n", noticia.Title);
			}
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


void TesteMetodos(){
	ListaNoticias lista = NewListaNoticias(10); //O capacity pode por qualquer coisa. Acho que 10 tah bom para nao ficar dando realloc nem gastar infinito memoria. Mas se passar disso ele realoca.
	DicionarioNoticia dic = NewDicionarioNoticia();

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

	AdicionarChave(&dic, "title", "Titulo"); //Aqui devem dar todos certos
	AdicionarChave(&dic, "abstract", "resumo");
	AdicionarChave(&dic, "author", "autor");
	AdicionarChave(&dic, "image", "imagem");
	AdicionarChave(&dic, "source", "fonte");
	AdicionarChave(&dic, "date", "data");
	AdicionarChave(&dic, "text", "um texto mais longo");

	AdicionarChave(&dic, "title", "Titulo"); //Aqui devem dar todos errados
	AdicionarChave(&dic, "abstract", "resumo");
	AdicionarChave(&dic, "author", "autor");
	AdicionarChave(&dic, "image", "imagem");
	AdicionarChave(&dic, "source", "fonte");
	AdicionarChave(&dic, "date", "data");
	AdicionarChave(&dic, "text", "um texto mais longo");

	CleanDicionarioNoticia(&dic);

}

void TesteGeraHtml(){
	ListaNoticias listaNoticias = NewListaNoticias(5);

	Noticia not1 = NewNoticia("head1", "Teste 1", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla in neque at magna tempus consectetur at ut ligula. Proin imperdiet a urna sit amet porta. Suspendisse sapien nulla, dapibus ut aliquet sit amet, aliquam ac tortor. Donec sed dictum justo. Ut tortor tortor, porta ut tincidunt ac, cursus ut lectus. Sed tempor, tortor vitae blandit suscipit, lacus nibh molestie magna, ac vestibulum felis dui non purus. Aliquam faucibus, dui quis laoreet lobortis, orci elit facilisis dui, quis pellentesque dui eros id velit. Maecenas tincidunt arcu ac leo semper convallis. Quisque ultrices mauris a orci aliquet adipiscing. Nam sagittis dui volutpat eleifend sollicitudin. Cras nec aliquet risus, sed sagittis eros. Sed quis sagittis mi, sit amet imperdiet neque. Ut mattis est est, ut pretium lorem tincidunt eget. Nam aliquam mollis sagittis. ", "Eu mermo", "Hoje", "imgTeste3.jpg", "sem fonte", "Etiam eu libero at ipsum lacinia dapibus id eu dolor. Nam volutpat vel lectus non mattis. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. In arcu lorem, pellentesque pellentesque fermentum vel, fringilla at dui. Maecenas bibendum, libero id fringilla venenatis, sem elit tincidunt eros, vitae posuere massa erat accumsan odio. Proin fermentum porta diam, ac condimentum urna euismod nec. Praesent nec ligula a turpis consectetur convallis ut vitae metus. Etiam at ligula scelerisque, mollis sapien non, facilisis erat. Duis eu adipiscing erat. Etiam consectetur feugiat nulla id euismod. Aliquam sed aliquam magna. Vestibulum posuere, velit eu interdum feugiat, dolor sapien faucibus tellus, in adipiscing neque ipsum porttitor orci. Aenean semper magna mi, vitae volutpat elit tincidunt sed. Curabitur dui quam, mollis nec condimentum hendrerit, vulputate eu magna. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas", 2);
	
	//MarcarMostrarObjetoNaNoticia(&not1, Title);
	//MarcarMostrarObjetoNaNoticia(&not1, Image);
	//MarcarMostrarObjetoNaNoticia(&not1, Abstract);
	MostrarPropriedade(&not1, "TITLE");
	MostrarPropriedade(&not1, "abstract");
	MostrarPropriedade(&not1, "Author");

	AppendElemento(&listaNoticias, not1);

	Noticia not2 = NewNoticia("head2", "Teste 2", "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Cras mauris arcu, gravida sed sagittis non, mattis in tellus. Donec imperdiet, nulla id iaculis tempus, leo odio eleifend erat, at gravida enim neque vel ligula. Integer non faucibus dui, vel laoreet velit. Morbi ornare nulla pretium orci ornare dictum. Vivamus dictum ipsum ac venenatis pulvinar. Curabitur venenatis lorem ut neque vulputate ornare. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Vestibulum ornare, quam non faucibus condimentum, turpis nulla tempor purus, a mattis sem velit quis lorem. Phasellus fringilla gravida placerat. In metus urna, molestie ac ornare in, vulputate sed metus. Nulla facilisi. Integer sollicitudin ultrices nunc, in ullamcorper orci dignissim nec. Maecenas id mi non diam gravida dictum. Nulla nulla elit, pellentesque ut mauris at, accumsan tincidunt felis. Nulla facilisi. ", "Eu mermo denovo", "Hoje", "imgTeste2.png", "sem fonte", "Praesent at feugiat nisl. Etiam id erat at sem porta pulvinar in tempus lorem. Quisque vel sollicitudin risus. Phasellus tristique nulla et ipsum cursus scelerisque. Proin nulla libero, ullamcorper sed suscipit quis, iaculis et mi. Sed ornare, urna sed tristique iaculis, ipsum dolor bibendum eros, sed aliquet felis lorem ut erat. Vivamus convallis ipsum ut dolor cursus, sed tincidunt quam auctor. Pellentesque ut justo porttitor, consectetur magna ornare, dignissim sapien. Phasellus eu nulla tellus. Donec vulputate sodales quam, eget viverra lorem eleifend sed. Morbi rutrum malesuada venenatis. ", 1);

	MarcarMostrarObjetoNaNoticia(&not2, Abstract);
	MarcarMostrarObjetoNaNoticia(&not2, Image);
	MarcarMostrarObjetoNaNoticia(&not2, Title);
	AppendElemento(&listaNoticias, not2);
	
	MarcarNoticiaParaExibicao(&listaNoticias, "HEAD2");
	MarcarNoticiaParaExibicao(&listaNoticias, "HEAD1");

	ImprimePaginaWeb("saidaTeste.html", &listaNoticias, 3);
}





int main(){

	TesteMetodos();

	DicionarioNoticia dic;
	
	//TesteGeraHtml();

}