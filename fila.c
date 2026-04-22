#include "fila.h"
#include <stdlib.h>

Fila* criarFila(int capacidade) {
    Fila* f = malloc(sizeof(Fila)); // Aloca memória para a estrutura da fila
    f->dados = malloc(sizeof(void*) * capacidade); // Aloca memória para os dados da fila
    f->inicio = 0; // Índice do início da fila
    f->fim = 0; // Índice do fim da fila
    f->tamanho = 0; // Tamanho atual da fila
    f->capacidade = capacidade; // Capacidade máxima da fila
    return f; // Retorna o ponteiro para a fila criada
}
// Verifica se a fila está vazia
int filaVazia(Fila* f) { 
    return f->tamanho == 0;
}
// Verifica se a fila está cheia
int enfileirar(Fila* f, void* item) {
    if (f->tamanho == f->capacidade) return 0; // Fila cheia, não é possível enfileirar

    f->dados[f->fim] = item; // Adiciona o item no fim da fila
    f->fim = (f->fim + 1) % f->capacidade; // Atualiza o índice do fim (circular)
    f->tamanho++; // Incrementa o tamanho da fila
    return 1; // Enfileiramento bem-sucedido
}
// Remove e retorna o item do início da fila
void* desenfileirar(Fila* f) {
    if (filaVazia(f)) return NULL; // Fila vazia, não é possível desenfileirar
// Armazena o item do início da fila, atualiza o índice do início e decrementa o tamanho
    void* item = f->dados[f->inicio]; // Armazena o item a ser retornado
    f->inicio = (f->inicio + 1) % f->capacidade; // Atualiza o índice do início (circular)
    f->tamanho--; // Decrementa o tamanho da fila
    return item; // Retorna o item desenfileirado
}
// Libera a memória alocada para a fila
void liberarFila(Fila* f) {
    free(f->dados); // Libera a memória alocada para os dados da fila
    free(f); // Libera a memória alocada para a estrutura da fila
}