#ifndef ARVORE_UTILS_H 
#define ARVORE_UTILS_H 
#include "fila.h"
#include "codigo.h"

typedef struct No {
    U8 byte;
    unsigned long long freq;
    struct No *esq, *dir;
} No; // nó da árvore de Huffman

void compactar(const char* entrada, const char* saida); // função de compactação
void descompactar(const char* entrada, const char* saida); // função de descompactação

#endif