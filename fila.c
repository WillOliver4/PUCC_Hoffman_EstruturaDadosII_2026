#ifndef FILACI_H
#define FILACI_H

typedef enum {
    TIPO_INT,  
    TIPO_CHAR
} TipoDado;

typedef struct {
    TipoDado tipo;
    int valor_int;
    char valor_char;
} Elemento;

typedef struct {
    int inicio;
    int fim;
    int qtdAtual;
    int capacidade;
    Elemento *elem;
} FilaCI;

FilaCI* criarFilaCI(int qtd);
int filaCheiaCI(FilaCI *fila);
int enfileirarCI(FilaCI *fila, TipoDado tipo, int num, char op);
int filaVaziaCI(FilaCI *fila);
Elemento desenfileirarCI(FilaCI *fila);
FilaCI* liberarMemoriaFilaCI(FilaCI *fila);

#endif
