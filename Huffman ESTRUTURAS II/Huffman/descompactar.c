#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "descompactar.h"
#include "compactar.h"

void descompactar_arquivo(const char *nome_entrada, const char *nome_saida) {
	
    //Ler frequências do cabeçalho
    FILE *entrada = abrir_arquivo_entrada(nome_entrada, "rb");
    if (entrada == NULL) {
        exit(1);
    }
    
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    
    // Ler o tamanho original e a tabela de frequências do arquivo compactado
    U64 tamanho_original;
    if (fread(&tamanho_original, sizeof(U64), 1, entrada) != 1) {
        printf("Erro ao ler tamanho original\n");
        exit(1);
    }

    for (U16 i = 0; i < 256; i++) {
        U64 freq;
        if (fread(&freq, sizeof(U64), 1, entrada) != 1) {
            printf("Erro ao ler frequências\n");
            exit(1);
        }
        
        if (freq > 0) {
            Elemento elem;
            elem.byte = (U8)i;
            elem.frequencia = freq;
            
            if (!novo_no_de_arvore_binaria(&(tab.vetor[i]), NULL, elem, NULL)) {
                printf("Erro ao criar nó da árvore\n");
                exit(1);
            }
            tab.quantidade_de_posicoes_preenchidas++;
        }
    }
    
    //Reconstrução da árvore de Huffman
    NoHuffman *raiz = NULL;
    construir_arvore_huffman(&tab, &raiz);
    
    //Descompactação dos dados
    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        exit(1);
    }
    
    NoHuffman *no_atual = raiz;
    U8 byte;
    int bit_pos = 7; // Começa pelo bit mais significativo
    U64 bytes_escritos = 0;

    // Navegação pela árvore até encontrar um byte original
    while (fread(&byte, sizeof(U8), 1, entrada) == 1 && bytes_escritos < tamanho_original) {
        while (bit_pos >= 0 && bytes_escritos < tamanho_original) {
            U8 bit = (byte >> bit_pos) & 1;
            bit_pos--;
            
            if (bit == 0) {
                no_atual = no_atual->esquerda;
            } else {
                no_atual = no_atual->direita;
            }
            
            // Quando chega em uma folha, escreve o byte original
            if (no_atual->esquerda == NULL && no_atual->direita == NULL) {
                fwrite(&(no_atual->byte), sizeof(U8), 1, saida);
                bytes_escritos++;
                no_atual = raiz;
            }
        }
        bit_pos = 7; // Reinicia para o próximo byte
    }
    
    // Limpeza final
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}