#ifndef COMPACTAR_H
#define COMPACTAR_H

#include <stdint.h>
#include "huffman.h"

// Função para compactar um arquivo
void compactar_arquivo(const char *nome_entrada, const char *nome_saida);

// Função auxiliar para abrir arquivo de entrada
FILE *abrir_arquivo_entrada(const char *nome_arquivo, const char *modo);

#endif