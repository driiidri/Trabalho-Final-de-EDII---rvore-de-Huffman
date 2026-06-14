//Trabalho final Arvore de huffman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Um nó da ÁRVORE. Ele sabe:
// - qual caractere ele representa (se for folha)
// - qual a frequência desse caractere
// - quem são seus filhos (esquerda e direita)

typedef struct No{
    char caractere;
    int frequencia;
    struct No *esquerda;
    struct No *direita;
} No;


// Um nó da LISTA. Ele sabe:
// - para qual árvore ele está apontando
// - quem é o próximo elemento da lista
//
// Pensa assim: a lista é uma fila de árvores ordenadas.
// Cada "vaga" dessa fila é um NoLista.
// Cada vaga segura UMA árvore inteira (pelo ponteiro *arvore)
// e aponta para a próxima vaga (pelo ponteiro *proximo)
typedef struct NoLista{
    No *raiz;
    struct NoLista *proximo;
} NoLista;

// Exemplo visual:
//
// lista --> [NoLista] --> [NoLista] --> [NoLista] --> NULL
//               |              |              |
//            [No 'a']      [No 'b']      [No 'r']
//            freq: 1       freq: 2       freq: 2

// Variável global da lista (floresta)
NoLista *lista = NULL;

//Aqui foi passado const char *arquivo pq char significa ponteiro para caractere q em c é pra representar uma  string, 
//o const é mais uma proteçção, ele diz para o compilador que a função pode ler o texto mas não modficar ele
//e o arquivo é o nome q vc deu para o ponteiro só
void contarFrequencias(const char *arquivo, int freq[256]){

    //Zera todas as 256 posições pq pode ter lixo de memória dentro
    for(int i = 0; i < 256; i++){
        freq[i] = 0;
    }

    FILE *fp = fopen(arquivo,  "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo!\n");
        return;
    }

    int c;
    while( (c = fgetc(fp)) != EOF){
        freq[c]++;
    }
    fclose(fp);
}




void inserirOrdenado(NoLista **lista, No *novo){

    NoLista *novoElemento = (NoLista *) malloc(sizeof(NoLista)); //novoElemento é um poneteiro para Nolista
    if(novoElemento == NULL){
        printf("Erro na alocação de memória!");
        return;
    }

    
    novoElemento->raiz = novo;
    novoElemento->proximo = NULL;

    //se a lista ta vazia o novo elemento vira a cabeça
   if(*lista == NULL){
    *lista = novoElemento;
    return;
   }

   No *cabeca = (*lista)->raiz;

   //ver se ele deve entrar antes da cabeça atual

   if(novo->frequencia < cabeca->frequencia || novo->frequencia == cabeca->frequencia 
    && novo->caractere < cabeca->caractere ){
        novoElemento->proximo = *lista;
        *lista = novoElemento;
        return;
    }

    //se não entrou em nenhum daqueles ele vai tentar acharr sua posição
    //Quando o while termina, seja por break ou por chegar no fim da lista, o anterior é quem vem antes e o atual é quem vem depois. O novo entra no meio dos dois.
    NoLista *anterior = *lista;
    NoLista *atual = (*lista)->proximo;

    while(atual != NULL){
        if(novo->frequencia < atual->raiz->frequencia || novo->frequencia == atual->raiz->frequencia &&
    novo->caractere < atual->raiz->caractere ){
         break;
    }
    anterior = atual;
   atual = atual->proximo;
    
}

novoElemento->proximo = atual;
anterior->proximo = novoElemento;


}


No *montarArvore(){
     while(lista != NULL && lista->proximo != NULL){

        NoLista *primeiroElemento = lista;
        No *primeiro = primeiroElemento->raiz;
        lista = lista->proximo;


        NoLista *segundoElemento = lista;
        No *segundo = segundoElemento->raiz;
        lista = lista->proximo;

        free(primeiroElemento);
        free(segundoElemento);


        No *pai = (No *) malloc(sizeof(No));
        pai->caractere = '\0';
        pai->frequencia = primeiro->frequencia + segundo->frequencia; 
        pai->esquerda = primeiro;
        pai->direita = segundo;

        inserirOrdenado(&lista, pai);
}


if(lista != NULL){
    return lista->raiz;
}

return NULL;
}



void gerarCodigos(No *no, char *caminhoAtual, int nivel, char dicionario[256][MAX]){
    // Caso base de segurança caso o nó for nulo, já encerra a recursão
    if(no == NULL) return;

    // Se o nó é uma folha, encontramos um caracter
    if(no->esquerda == NULL && no->direita == NULL){
        caminhoAtual[nivel] = '\0'; // Finaliza a string do código
        // O caminho até aqui é o código de Huffman deste caracter
        strcpy(dicionario[(unsigned char)no->caractere], caminhoAtual);
        return;
    }
    // Caso contrário continua percorrendo a árvore:


    // Vai adicionamos '0' ao código
        caminhoAtual[nivel] = '0';
        gerarCodigos(no->esquerda, caminhoAtual, nivel + 1, dicionario);
    
    // Vai para a direita, adicionamos '1' ao código
        caminhoAtual[nivel] = '1';
        gerarCodigos(no->direita, caminhoAtual,  nivel + 1, dicionario);
    
}

void comprimir(char dicionario[256][MAX]){
    int c;

    // Abertura do arquivo origem
    FILE *entrada = fopen("amostra.txt", "r");
    if(entrada == NULL) return;

    // Abertura do arquivo de destino
    FILE *saida = fopen("codificado.txt", "w");
    if(saida == NULL){
        fclose(entrada);
        return;
    }

    // Processo de compressão
    while((c = fgetc(entrada)) != EOF){
        fprintf(saida, "%s", dicionario[(unsigned char)c]);
    }

    // Finalizando
    fclose(entrada);
    fclose(saida);
}

void descompressao(No *raiz){
    No *aux = raiz;
    int bit;

    // Abertura do arquivo com o texto codificado
    FILE *entrada = fopen("codificado.txt", "r");
    if(entrada == NULL) return;

    // Abertura do arquivo de destino
    FILE *saida = fopen("decodificado.txt", "w");
    if(saida == NULL){
        fclose(entrada);
        return;
    }

    // Processo de decodificação
    while((bit = fgetc(entrada)) != EOF){
        // Navegação baseada no bit lido
        if(bit == '0'){
            aux = aux->esquerda;
        }
        else if(bit == '1'){
            aux = aux->direita;
        }

        // Verificação do Nó Folha, onde estão os caracteres
        if(aux->esquerda == NULL && aux->direita == NULL){
            // Escreve o caracter original no arquivo de saída
            fprintf(saida, "%c", aux->caractere);
            // Volta para a raiz depois de salvar o caracter
            aux = raiz;
        }
    }

    // Finalizando
    fclose(entrada);
    fclose(saida);

}








int main(){
    int freq[256] = {0};

    contarFrequencias("amostra.txt", freq );
 for(int i = 0; i < 256; i++){
       if ( freq[i] > 0){
        No *novo = (No *) malloc(sizeof(No));
        novo->caractere = (char) i;
        novo->frequencia = freq[i];
        novo->esquerda = NULL;
        novo->direita = NULL;
        inserirOrdenado(&lista, novo);
       }
    }
    
    No *topo = montarArvore();

    char dicionario[256][MAX];
    char caminho[MAX];
    gerarCodigos(topo, caminho, 0, dicionario);

    comprimir(dicionario);

    descompressao(topo);

    return 0;

}