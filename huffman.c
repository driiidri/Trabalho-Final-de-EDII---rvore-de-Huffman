//Trabalho final Arvore de huffman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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















int main(){
    int freq[256] = {0};

    contarFrequencias("amostra.txt", freq );

}