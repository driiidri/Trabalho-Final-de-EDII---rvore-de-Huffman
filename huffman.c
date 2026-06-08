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
    No *arvore;
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
    for(int i; i < 256; i++){
        freq[i] = 0;
    }

    FILE *fp = fopen(arquivo,  "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo!\n");
        return;
    }

    int c;
    while( c = fgetc(fp) != EOF){
        freq[c]++;
    }
    fclose(fp);
}




void inserirOrdenado(){
    
}


void gerarCodigos(No *no, char *caminhoAtual, int nivel, char dicionario[MAX]){
    // Caso base de segurança caso o nó for nulo, já encerra a recursão
    if(no == NULL) return;

    // Se o nó é uma folha, encontramos um caracter
    if(no->esquerda == NULL && no->direita == NULL){
        caminhoAtual[nivel] = '\0' // Finaliza a string do código
        // O caminho até aqui é o código de Huffman deste caracter
        strcpy(dicionario[(usigned char)no->caracter], caminhoAtual);
        return;
    }
    // Caso contrário continua percorrendo a árvore:
    // Tenta ir para a esquerda, adicionamos '0' ao código
    else if(no->esquerda != NULL){
        caminhoAtual[nivel] = '0';
        gerarCodigos(no->esquerda, caminhoAtual, nivel + 1, dicionario);
    }
    // Tenta ir para a direita, adicionamos '1' ao código
    else if(no->direita != NULL){
        caminhoAtual[nivel] = '1';
        gerarCodigos(no.direita, caminhoAcumulado + "1", dicionario);
    }
}

void comprimir(char **dicionario){
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
        // Converte o caracter lido para seu índice correspondente no dicionário
        char *caminho = dicionario[(usinged char)c];
        // Grava a representação binária no arquivo de saída
        if(caminho != NULL) fprintf(saida, "%s", caminho);
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
            fprintf(saida, "%c", aux->caracter);
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

}