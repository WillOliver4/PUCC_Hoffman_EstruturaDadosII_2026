#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// Cria a fila
FilaCI* criarFilaCI(int qtd) {
    FilaCI *fila = (FilaCI*) malloc(sizeof(FilaCI));
    if (fila == NULL) return NULL;

    fila->elem = (Elemento*) malloc(qtd * sizeof(Elemento));
    if (fila->elem == NULL) {
        free(fila);
        return NULL;
    }

    fila->inicio = 0;
    fila->fim = -1;
    fila->qtdAtual = 0;
    fila->capacidade = qtd;

    return fila;
}
//Verifica se a fila está cheia
int filaCheiaCI(FilaCI *fila) {
    return (fila->qtdAtual == fila->capacidade);
}

//Verifica se a fila está vazia
int filaVaziaCI(FilaCI *fila) {
    return (fila->qtdAtual == 0);
}

//Insere elemento na fila (circular)
int enfileirarCI(FilaCI *fila, TipoDado tipo, int num, char op) {
    if (fila == NULL || filaCheiaCI(fila))
        return 0;

    // Avança circularmente
    fila->fim = (fila->fim + 1) % fila->capacidade;

    fila->elem[fila->fim].tipo = tipo;

    if (tipo == TIPO_INT)
        fila->elem[fila->fim].valor_int = num;
    else
        fila->elem[fila->fim].valor_char = op;

    fila->qtdAtual++;

    return 1;
}
//Remove elemento da fila
Elemento desenfileirarCI(FilaCI *fila) {
    Elemento vazio;

    if (fila == NULL || filaVaziaCI(fila)) {
        // Retorno padrão (evita lixo)
        vazio.tipo = TIPO_INT;
        vazio.valor_int = 0;
        return vazio;
    }

    Elemento removido = fila->elem[fila->inicio];

    // Avança circularmente
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->qtdAtual--;

    return removido;
}

//Libera memória da fila
FilaCI* liberarMemoriaFilaCI(FilaCI *fila) {
    if (fila != NULL) {
        if (fila->elem != NULL)
            free(fila->elem);

        free(fila);
    }
    return NULL;
}