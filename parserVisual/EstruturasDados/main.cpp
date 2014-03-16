#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

enum  {
	Title, Abstract, Author, Date, Image, Source, Text
};

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
	short mascaraPropriedades[7];
} Noticia;

typedef struct {
	int capacidade;
	int tamanho;
	Noticia * valores;
}ListaNoticias;

//Os argumentos precisam ser strings bem formadas (com NUL no final). Todos os objetos sao reinstanciados, ou seja, pode dar free nos fontes depois
//Nao mudar abstrac para abstract. Palavra reservada.
Noticia NewNoticia(char * nomeObjeto, char * title, char * abstrac, char * author, char * date, char * image, char * source, char * text, int numCol){
	Noticia retorno;
	int i = 0;

	retorno.NomeObjeto = (char *)calloc(strlen(nomeObjeto) + 1, sizeof(char));
	strcpy(retorno.NomeObjeto, nomeObjeto);

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
		retorno.mascaraPropriedades[i] = 0;
	}

	retorno.buscada = 0;
	retorno.posicaoNoticia = -1;
	return retorno;
}

//Marca um dos itens da mascara de objetos a serem mostrados pela noticia
void MarcarMostrarObjetoNaNoticia(Noticia * noticia, int tipoObjeto){
	if (tipoObjeto == Title)
		(*noticia).mascaraPropriedades[0] = 1;
	else if (tipoObjeto == Abstract)
		(*noticia).mascaraPropriedades[1] = 1;
	else if (tipoObjeto == Author)
		(*noticia).mascaraPropriedades[2] = 1;
	else if (tipoObjeto == Date)
		(*noticia).mascaraPropriedades[3] = 1;
	else if (tipoObjeto == Image)
		(*noticia).mascaraPropriedades[4] = 1;
	else if (tipoObjeto == Source)
		(*noticia).mascaraPropriedades[5] = 1;
	else if (tipoObjeto == Text)
		(*noticia).mascaraPropriedades[6] = 1;
}

//Sinaliza que a noticia apontada deve ser impressa no final.
//nomeToken eh o nome da noticia digitado no codigo que vem do token
//Se nao achar a noticia na lista joga no stderror que o usuario digitou alguma besteira
//Deve ser chamado na ordem do structure da news, pois a ordem de chamada influencia
//na hora da leitura
void MarcarNoticiaParaExibicao(ListaNoticias * listaNoticias, char * nomeToken){
	int ultimaNoticia = -2;
	int i = 0;
	for (i = 0; i < (*listaNoticias).tamanho; i++){ //pega a posicao atribuida da ultima noticia
		if ((*listaNoticias).valores[i].posicaoNoticia > ultimaNoticia)
			ultimaNoticia = (*listaNoticias).valores[i].posicaoNoticia;
	}
	ultimaNoticia++; //Representa a posicao da proxima noticia

	for (i = 0; i < (*listaNoticias).tamanho; i++){
		if (strcmp((*listaNoticias).valores[i].NomeObjeto, nomeToken) == 0 && (*listaNoticias).valores[i].posicaoNoticia == -1){
			(*listaNoticias).valores[i].posicaoNoticia = ultimaNoticia;
			return;
		}
	}

	fprintf(stderr, "\nWarning: Noticia %s nao faz parte das noticias declaradas\n", nomeToken);
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
//Mantem a ordem digitada pelo usuario, mas respeita o numero de colunas desejado.
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
void ImprimeUmaNoticia(Noticia noticia, FILE * arquivo){
	return;
}


void ImprimeTodasNoticias(ListaNoticias * listaNoticias, int colspan, FILE * arquivo){ //Isso daqui vai ser chamado quando ele reduzir o newspaper, entao jah vou ter essa informacao do structure do newspaper
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
	} while (proximaNoticia != NULL);
}


int main(){
	ListaNoticias lista = NewListaNoticias(10); //O capacity pode por qualquer coisa. Acho que 10 tah bom para nao ficar dando realloc nem gastar infinito memoria. Mas se passar disso ele realoca.

	Noticia not10 = NewNoticia("headline10", "titulo10", "abstract10", "author10", "data10", "imagem10", "fonte10", "texto balbalablab10", 10); //Instancias de teste
	Noticia not1 = NewNoticia("headline1", "titulo1", "abstract1", "author1", "data1", "imagem1", "fonte1", "texto balbalablab10", 1);
	
	MarcarMostrarObjetoNaNoticia(&not10, Title); //Marca que a noticia not10 deve mostrar titulo e source
	MarcarMostrarObjetoNaNoticia(&not10, Source);

	MarcarMostrarObjetoNaNoticia(&not1, Abstract); //Marca que noticia not1 deve mostrar abstract e author
	MarcarMostrarObjetoNaNoticia(&not1, Author);

	AppendElemento(&lista, not10); //Coloca not10 e not1 na lista
	AppendElemento(&lista, not1);

	MarcarNoticiaParaExibicao(&lista, "headline10");
	MarcarNoticiaParaExibicao(&lista, "headline1");

	int jornalOverflow = TestaPorMaiorColSpan(&lista, 7); //Deve retornar 1, pois tem uma noticia com 10 lah
	int jornalOverflow2 = TestaPorMaiorColSpan(&lista, 10); //Deve retornar falso, maior noticia tem 10 colunas 

	int aindaNaoPesquisouTudo = !TestaSeTodasNoticiasJahForamBuscadas(&lista); //Tem de dar 1 (negado de 0), pois nenhuma noticia ainda foi eleita para ser impressa

	Noticia * not10ret = BuscaProximaNoticia(&lista); //Escolhe a primeira noticia disponivel
	Noticia * not1ret = BuscaProximaNoticia(&lista); //Escolha a segunda noticia disponivel 

	Noticia * not1ret2 = BuscaProximaNoticia(&lista); // Deve voltar NULL, pois as duas noticias jah foram pesquisadas e nao tem mais nada sem pesquisar

	int jahPesquisouTudo = TestaSeTodasNoticiasJahForamBuscadas(&lista); //Deve retornar 1, pois as duas noticias jah foram eleitas



}