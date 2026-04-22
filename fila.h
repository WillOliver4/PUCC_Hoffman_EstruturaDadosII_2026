#ifndef FILA_H
#define FILA_H

typedef struct {
    void **dados;
    int inicio, fim, tamanho, capacidade;
} Fila; // Estrutura da fila

Fila* criarFila(int capacidade); // Cria uma fila com capacidade inicial
int filaVazia(Fila* f); // Verifica se a fila está vazia
int enfileirar(Fila* f, void* item); // Insere um elemento na fila
void* desenfileirar(Fila* f); // Remove um elemento da fila
void liberarFila(Fila* f); // Libera a memória ocupada pela fila

#endif