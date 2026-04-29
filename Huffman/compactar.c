#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compactar.h"

FILE *abrir_arquivo_entrada(const char *nome_arquivo, const char *modo) {
    FILE *arquivo = fopen(nome_arquivo, modo);
    if (arquivo != NULL) return arquivo;

    char caminho[512];
    snprintf(caminho, sizeof(caminho), "..\\%s", nome_arquivo);
    arquivo = fopen(caminho, modo);
    if (arquivo != NULL) return arquivo;

    snprintf(caminho, sizeof(caminho), "..\\..\\%s", nome_arquivo);
    arquivo = fopen(caminho, modo);
    if (arquivo != NULL) return arquivo;

    fprintf(stderr, "Erro ao abrir arquivo de entrada: %s\n", nome_arquivo);
    perror("fopen");
    return NULL;
}

void compactar_arquivo(const char *nome_entrada, const char *nome_saida) {
	
    //Calcular frequências dos bytes no arquivo
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    
    FILE *entrada = abrir_arquivo_entrada(nome_entrada, "rb");
    if (entrada == NULL) {
        exit(1);
    }
    
    // Contar a frequência de cada byte
    U8 byte;
    U64 tamanho_original = 0;
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        if (!inclua_byte(byte, &tab)) {
            printf("Erro ao incluir byte na tabela\n");
            exit(1);
        }
        tamanho_original++;
    }
    fclose(entrada);
    
    //Construir árvore de Huffman
    NoHuffman *raiz = NULL;
    construir_arvore_huffman(&tab, &raiz);
    
    //Gerar códigos para cada byte
    Codigo *tabela_codigos[256] = {NULL};
    Codigo codigo_atual;
    if (!novo_codigo(&codigo_atual)) {
        printf("Erro ao criar código\n");
        exit(1);
    }
    
    gerar_codigos(raiz, &codigo_atual, tabela_codigos);
    free_codigo(&codigo_atual);
    
    //Escrever arquivo compactado
    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        exit(1);
    }
    
    // Escreve cabeçalho com o tamanho original e as frequências para reconstrução da árvore
    fwrite(&tamanho_original, sizeof(U64), 1, saida);
    for (U16 i = 0; i < 256; i++) {
        U64 freq = 0;
        if (tab.vetor[i] != NULL) {
            freq = tab.vetor[i]->informacao.frequencia;
        }
        fwrite(&freq, sizeof(U64), 1, saida);
    }
    
    //Compactar os dados
    entrada = abrir_arquivo_entrada(nome_entrada, "rb");
    if (entrada == NULL) {
        exit(1);
    }
    
    Codigo buffer;
    if (!novo_codigo(&buffer)) {
        printf("Erro ao criar buffer\n");
        exit(1);
    }
    
    U8 byte_escrito = 0;
    int bits_escritos = 0;
    
    // Para cada byte do arquivo original, escreve seu código correspondente
    while (fread(&byte, sizeof(U8), 1, entrada)) {
        Codigo *codigo = tabela_codigos[byte];
        for (U8 i = 0; i < codigo->tamanho; i++) {
            int byte_index = i / 8;
            int bit_index = i % 8;
            U8 bit = (codigo->byte[byte_index] >> (7 - bit_index)) & 1;
            
            byte_escrito = (byte_escrito << 1) | bit;
            bits_escritos++;
            
            if (bits_escritos == 8) {
                fwrite(&byte_escrito, sizeof(U8), 1, saida);
                bits_escritos = 0;
                byte_escrito = 0;
            }
        }
    }
    
    // Escreve bits restantes não completos
    if (bits_escritos > 0) {
        byte_escrito <<= (8 - bits_escritos);
        fwrite(&byte_escrito, sizeof(U8), 1, saida);
    }
    
    // Limpeza final
    fclose(entrada);
    fclose(saida);
    free_codigo(&buffer);
    liberar_arvore(raiz);
    for (U16 i = 0; i < 256; i++) {
        if (tabela_codigos[i] != NULL) {
            free_codigo(tabela_codigos[i]);
            free(tabela_codigos[i]);
        }
    }
}