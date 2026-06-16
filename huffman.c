// Trabalho final Arvore de huffman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct No
{
    char caractere;
    int frequencia;
    struct No *esquerda;
    struct No *direita;
} No;

typedef struct NoLista
{
    No *raiz;
    struct NoLista *proximo;
} NoLista;

NoLista *lista = NULL;

void contarFrequencias(const char *arquivo, int freq[256])
{

    // Zera todas as 256 posições pq pode ter lixo de memória dentro
    for (int i = 0; i < 256; i++)
    {
        freq[i] = 0;
    }

    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL)
    {
        printf("Erro na abertura do arquivo!\n");
        return;
    }

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        freq[c]++;
    }
    fclose(fp);
}

void inserirOrdenado(NoLista **lista, No *novo)
{

    NoLista *novoElemento = (NoLista *)malloc(sizeof(NoLista));
    if (novoElemento == NULL)
    {
        printf("Erro na alocação de memória!");
        return;
    }

    novoElemento->raiz = novo;

    novoElemento->proximo = NULL;

    if (*lista == NULL)
    {
        *lista = novoElemento;
        return;
    }

    NoLista *anterior = NULL;
    NoLista *atual = *lista;

    int novoEhFolha = (novo->esquerda == NULL && novo->direita == NULL);

    while (atual != NULL)
    {

        No *noAtual = atual->raiz;

        int atualEhFolha = (noAtual->esquerda == NULL && noAtual->direita == NULL);

        int deveEntrarAntes = 0;

        if (novo->frequencia < noAtual->frequencia)
        {

            deveEntrarAntes = 1;
        }
        else if (novo->frequencia == noAtual->frequencia)
        {

            if (!novoEhFolha && atualEhFolha)
            {

                deveEntrarAntes = 1;
            }
            else if (!novoEhFolha && !atualEhFolha)
            {

                deveEntrarAntes = 1;
            }
            else if (novoEhFolha && atualEhFolha)
            {
                if (novo->caractere < noAtual->caractere)
                {
                    deveEntrarAntes = 1;
                }
            }
        }

        if (deveEntrarAntes)
            break;
        anterior = atual;
        atual = atual->proximo;
    }

    novoElemento->proximo = atual;

    if (anterior == NULL)
    {
        *lista = novoElemento;
    }
    else
    {

        anterior->proximo = novoElemento;
    }
}

No *montarArvore()
{
    while (lista != NULL && lista->proximo != NULL)
    {

        NoLista *primeiroElemento = lista;
        No *primeiro = primeiroElemento->raiz;
        lista = lista->proximo;

        NoLista *segundoElemento = lista;
        No *segundo = segundoElemento->raiz;
        lista = lista->proximo;

        free(primeiroElemento);
        free(segundoElemento);

        No *pai = (No *)malloc(sizeof(No));
        pai->caractere = '\0';
        pai->frequencia = primeiro->frequencia + segundo->frequencia;
        pai->esquerda = primeiro;
        pai->direita = segundo;

        inserirOrdenado(&lista, pai);
    }

    if (lista != NULL)
    {
        return lista->raiz;
    }

    return NULL;
}

void gerarCodigos(No *no, char *caminhoAtual, int nivel, char dicionario[256][MAX])
{
    // Caso base de segurança caso o nó for nulo, já encerra a recursão
    if (no == NULL)
        return;

    // Se o nó é uma folha, encontramos um caracter
    if (no->esquerda == NULL && no->direita == NULL)
    {
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
    gerarCodigos(no->direita, caminhoAtual, nivel + 1, dicionario);
}

void comprimir(char dicionario[256][MAX])
{
    int c;

    // Abertura do arquivo origem
    FILE *entrada = fopen("amostra.txt", "r");
    if (entrada == NULL)
        return;

    // Abertura do arquivo de destino
    FILE *saida = fopen("codificado.txt", "w");
    if (saida == NULL)
    {
        fclose(entrada);
        return;
    }

    // Processo de compressão
    while ((c = fgetc(entrada)) != EOF)
    {
        fprintf(saida, "%s", dicionario[(unsigned char)c]);
    }

    // Finalizando
    fclose(entrada);
    fclose(saida);
}

void descompressao(No *raiz)
{
    No *aux = raiz;
    int bit;

    // Abertura do arquivo com o texto codificado
    FILE *entrada = fopen("codificado.txt", "r");
    if (entrada == NULL)
        return;

    // Abertura do arquivo de destino
    FILE *saida = fopen("decodificado.txt", "w");
    if (saida == NULL)
    {
        fclose(entrada);
        return;
    }

    // Processo de decodificação
    while ((bit = fgetc(entrada)) != EOF)
    {
        // Navegação baseada no bit lido
        if (bit == '0')
        {
            aux = aux->esquerda;
        }
        else if (bit == '1')
        {
            aux = aux->direita;
        }

        // Verificação do Nó Folha, onde estão os caracteres
        if (aux->esquerda == NULL && aux->direita == NULL)
        {
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

int main()
{
    int freq[256] = {0};

    contarFrequencias("amostra.txt", freq);
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            No *novo = (No *)malloc(sizeof(No));
            novo->caractere = (char)i;
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

void inserirOrdenado(NoLista **lista, No *novo)
{

    // Aloca memória para o novo elemento da lista (o "envelope" que vai guardar o nó da árvore)
    NoLista *novoElemento = (NoLista *)malloc(sizeof(NoLista));
    if (novoElemento == NULL)
    {
        printf("Erro na alocação de memória!");
        return;
    }

    novoElemento->raiz = novo;

    novoElemento->proximo = NULL;

    if (*lista == NULL)
    {
        *lista = novoElemento;
        return;
    }

    NoLista *anterior = NULL;
    NoLista *atual = *lista;

    // Verifica se o nó que estamos inserindo é uma FOLHA (não tem filhos)
    // Folha = representa um caractere real do texto
    // Nó interno = foi criado durante a montagem da árvore, une dois outros nós
    int novoEhFolha = (novo->esquerda == NULL && novo->direita == NULL);

    // Percorre a lista procurando a posição correta para o novo elemento
    // A lista deve ficar ordenada por frequência crescente
    while (atual != NULL)
    {

        // Pega o nó da árvore que está na posição atual da lista
        No *noAtual = atual->raiz;

        // Verifica se o nó da posição atual também é uma folha
        int atualEhFolha = (noAtual->esquerda == NULL && noAtual->direita == NULL);

        // Flag que decide se o novo elemento deve entrar ANTES do atual
        int deveEntrarAntes = 0;

        if (novo->frequencia < noAtual->frequencia)
        {
            // Regra 1: frequência menor sempre tem prioridade, entra antes
            deveEntrarAntes = 1;
        }
        else if (novo->frequencia == noAtual->frequencia)
        {
            // Frequências iguais: precisamos de critério de desempate
            if (!novoEhFolha && atualEhFolha)
            {
                // Regra 2: nó INTERNO empata com FOLHA → interno tem prioridade
                // Isso acontece quando um pai recém-criado volta para a lista
                // e encontra folhas de mesma frequência. Ele deve entrar antes
                // para que a árvore seja montada na ordem correta
                deveEntrarAntes = 1;
            }
            else if (!novoEhFolha && !atualEhFolha)
            {
                // Regra 3: dois nós INTERNOS empatam → o mais novo entra antes
                // (comportamento LIFO para internos: o último criado tem prioridade)
                // Isso garante que pais criados mais recentemente sejam unidos
                // primeiro, gerando a estrutura de árvore esperada pelo professor
                deveEntrarAntes = 1;
            }
            else if (novoEhFolha && atualEhFolha)
            {
                // Regra 4: duas FOLHAS empatam → desempata pela tabela ASCII
                // Caractere de menor valor ASCII entra primeiro
                // Exemplo: 'a'(97) entra antes de 'b'(98)
                if (novo->caractere < noAtual->caractere)
                {
                    deveEntrarAntes = 1;
                }
                // Se novo->caractere >= noAtual->caractere, não entra antes,
                // deveEntrarAntes continua 0 e seguimos procurando posição
            }

            // Caso não coberto: novo é FOLHA e atual é INTERNO com mesma frequência
            // Nesse caso o interno já está na frente e o novo (folha) não tem
            // prioridade, então deveEntrarAntes fica 0 e continuamos andando
        }

        // Se encontramos a posição certa, paramos o loop
        // O novo vai entrar entre 'anterior' e 'atual'
        if (deveEntrarAntes)
            break;
        // Ainda não achamos a posição: avança os dois ponteiros
        anterior = atual;
        atual = atual->proximo;
    }

    // Conecta o novo elemento à lista na posição encontrada:
    // O próximo do novo aponta para quem estava na posição atual
    novoElemento->proximo = atual;

    if (anterior == NULL)
    {
        // anterior == NULL significa que o novo deve entrar no INÍCIO da lista
        // (ou a lista tinha só um elemento e o novo tem prioridade sobre ele)
        *lista = novoElemento;
    }
    else
    {
        // Caso geral: novo entra no MEIO ou no FIM da lista
        // O anterior passa a apontar para o novo elemento
        anterior->proximo = novoElemento;
    }
}