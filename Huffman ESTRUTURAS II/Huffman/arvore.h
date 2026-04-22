#ifndef ARVORE
#define ARVORE
#endif
#include "codigo.h"
#include "tabela_de_frequencias.h"

typedef struct NoHuffman {
    U8 byte;
    U64 frequencia;
    struct NoHuffman *esquerda;
    struct NoHuffman *direita;
} NoHuffman;

NoHuffman *criar_no(U8 byte, U64 frequencia, NoHuffman *esquerda, NoHuffman *direita);
