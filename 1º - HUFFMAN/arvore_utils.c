#ifndef ARVORE_UTILS_H 
#define ARVORE_UTILS_H 
#include "arvore_utils.h"
#include <stdio.h>
#include <stdlib.h>

No* criarNo(U8 b, unsigned long long f, No* e, No* d) {
    No* n = malloc(sizeof(No));
    n->byte = b;
    n->freq = f;
    n->esq = e;
    n->dir = d;
    return n;
}

// versão simplificada (foco didático)
void compactar(const char* entrada, const char* saida) {
    FILE* in = fopen(entrada, "rb");
    FILE* out = fopen(saida, "wb");

    if (!in || !out) {
        printf("Erro arquivo\n");
        return;
    }

    // copia simples (placeholder didático)
    unsigned char buffer;
    while (fread(&buffer, 1, 1, in))
        fwrite(&buffer, 1, 1, out);

    fclose(in);
    fclose(out);
}

void descompactar(const char* entrada, const char* saida) {
    // mesma lógica simplificada
    compactar(entrada, saida);
}