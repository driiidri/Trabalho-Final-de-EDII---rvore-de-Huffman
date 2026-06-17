//Trabalho final Arvore de huffman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100



typedef struct No{
    char caractere;
    int frequencia;
    struct No *esquerda;
    struct No *direita;
} No;



typedef struct NoLista{
    No *raiz;
    struct NoLista *proximo;
} NoLista;


NoLista *lista = NULL;


void contarFrequencias(const char *arquivo, int freq[256]){

   
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




void inserirOrdenado(NoLista **lista, No *novo) {

    
    NoLista *novoElemento = (NoLista *) malloc(sizeof(NoLista));
    if (novoElemento == NULL) {
        printf("Erro na alocação de memória!");
        return;
    }

    
    novoElemento->raiz = novo;

     
    novoElemento->proximo = NULL;


     
    if (*lista == NULL) {
        *lista = novoElemento;
        return;
    }


    
    NoLista *anterior = NULL;
    NoLista *atual = *lista;


   
    int novoEhFolha = (novo->esquerda == NULL && novo->direita == NULL);


    
    while (atual != NULL) {

        
        No *noAtual = atual->raiz;

        
        int atualEhFolha = (noAtual->esquerda == NULL && noAtual->direita == NULL);

       
        int deveEntrarAntes = 0;


        if (novo->frequencia < noAtual->frequencia) {
             
            deveEntrarAntes = 1;
            } else if (novo->frequencia == noAtual->frequencia) {
           
    if (!novoEhFolha && atualEhFolha) {
       
        deveEntrarAntes = 1;
    } else if (!novoEhFolha && !atualEhFolha) {
          
        deveEntrarAntes = 1;
    } else if (novoEhFolha && atualEhFolha) {
        
        if (novo->caractere < noAtual->caractere) {
            deveEntrarAntes = 1;
        }
        
    }

       
}

       
        if (deveEntrarAntes) break;

        anterior = atual;
        atual = atual->proximo;
    }

  
    novoElemento->proximo = atual;

    if (anterior == NULL) {
       
        *lista = novoElemento;
    } else {
        
        anterior->proximo = novoElemento;
    }
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
    
    if(no == NULL) return;

   
    if(no->esquerda == NULL && no->direita == NULL){
        caminhoAtual[nivel] = '\0'; 
       
        strcpy(dicionario[(unsigned char)no->caractere], caminhoAtual);
        return;
    }
    


    
        caminhoAtual[nivel] = '0';
        gerarCodigos(no->esquerda, caminhoAtual, nivel + 1, dicionario);
    
    
        caminhoAtual[nivel] = '1';
        gerarCodigos(no->direita, caminhoAtual,  nivel + 1, dicionario);
    
}

void comprimir(char dicionario[256][MAX]){
    int c;

    
    FILE *entrada = fopen("amostra.txt", "r");
    if(entrada == NULL) return;

   
    FILE *saida = fopen("codificado.txt", "w");
    if(saida == NULL){
        fclose(entrada);
        return;
    }

    
    while((c = fgetc(entrada)) != EOF){
        fprintf(saida, "%s", dicionario[(unsigned char)c]);
    }

    
    fclose(entrada);
    fclose(saida);
}

void descompressao(No *raiz){
    No *aux = raiz;
    int bit;

    
    FILE *entrada = fopen("codificado.txt", "r");
    if(entrada == NULL) return;

    
    FILE *saida = fopen("decodificado.txt", "w");
    if(saida == NULL){
        fclose(entrada);
        return;
    }

    
    while((bit = fgetc(entrada)) != EOF){
        
        if(bit == '0'){
            aux = aux->esquerda;
        }
        else if(bit == '1'){
            aux = aux->direita;
        }

        
        if(aux->esquerda == NULL && aux->direita == NULL){
            
            fprintf(saida, "%c", aux->caractere);
            
            aux = raiz;
        }
    }

   
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