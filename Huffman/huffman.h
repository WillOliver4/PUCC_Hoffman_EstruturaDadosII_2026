#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include "codigo.h"
#include "tabela_de_frequencias.h"

// Estrutura para representar um nó da árvore de Huffman
// U8 é um tipo definido em meustiposonlinegdb.h que representa um byte (unsigned char), e U64 é um tipo que representa uma frequência (unsigned long int).
// U64 é usado para armazenar a frequência de cada byte, permitindo contar até 2^64 - 1 ocorrências, o que é suficiente para arquivos muito grandes. A estrutura NoHuffman tem ponteiros para os filhos esquerdo e direito, permitindo a construção da árvore binária necessária para o algoritmo de Huffman.
typedef struct NoHuffman {
    U8 byte;                // Byte armazenado no nó
    U64 frequencia;         // Frequência do byte 
    struct NoHuffman *esquerda;  
    struct NoHuffman *direita;   
} NoHuffman;

// Funções principais
void construir_arvore_huffman(Tabela_de_frequencias *tab, NoHuffman **raiz);
void gerar_codigos(NoHuffman *no, Codigo *codigo_atual, Codigo *tabela_codigos[256]);
void liberar_arvore(NoHuffman *no);
NoHuffman *criar_no(U8 byte, U64 frequencia, NoHuffman *esquerda, NoHuffman *direita);

#endif